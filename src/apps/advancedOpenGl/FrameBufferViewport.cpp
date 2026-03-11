#include "FrameBufferViewport.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

#include <libs/logger/Logger.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

FrameBufferViewport::FrameBufferViewport(const std::string &name, float width,
                                         float height,
                                         const std::filesystem::path &assetsDir)
    : Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  m_shaderManager.addShader("cube", m_assetsDir / "shaders/cube.vert",
                            m_assetsDir / "shaders/cube.frag");

  m_shaderManager.addShader("framebuffer",
                            m_assetsDir / "shaders/frameBuffer.vert",
                            m_assetsDir / "shaders/frameBuffer.frag");

  m_shaderManager.addShader("grayscale",
                            m_assetsDir / "shaders/frameBuffer.vert",
                            m_assetsDir / "shaders/grayscale.frag");

  m_shaderManager.addShader("inversion",
                            m_assetsDir / "shaders/frameBuffer.vert",
                            m_assetsDir / "shaders/inversion.frag");

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", (m_assetsDir / "textures").string())};

  // TODO: move this to a FrameBuffer class
  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

  // Attach the texture to the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_textureColorBuffer.id(), 0);

  // Create a renderbuffer object for depth and stencil attachment (we won't be
  // sampling these)
  glGenRenderbuffers(1, &m_renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, m_renderBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Logger::logError("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferViewport::onImguiUpdate() {
  static std::vector<std::string> postProcessingEffects{
      "framebuffer", "grayscale", "inversion"};
  static std::string current_item = postProcessingEffects[0];

  ImGui::Begin("Frame Buffer Settings");

  if (ImGui::BeginCombo("post processing", current_item.c_str())) {
    for (const auto &item : postProcessingEffects) {
      bool is_selected = (current_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        current_item = item;
        m_currentShader = item;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::End();
}

void FrameBufferViewport::onEvent(libs::events::Event &event) {
  m_cameraController.onEvent(event);
}

bool FrameBufferViewport::onViewportResize(float newWidth, float newHeight) {
  m_textureColorBuffer.setSize(newWidth, newHeight);
  m_camera->setViewportSize(newWidth, newHeight);

  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth,
                        newHeight);

  return false;
}

void FrameBufferViewport::drawScene() {
  glDisable(GL_CULL_FACE);
  m_cameraController.update();

  glActiveTexture(GL_TEXTURE0);
  drawInFrameBuffer();

  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glViewport(m_xBottomLeft, m_yBottomLeft, m_width, m_height);
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);

  auto screenShader = m_shaderManager.getShader(m_currentShader);
  screenShader->use();

  glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer.id());
  screenShader->setInt("screenTexture", 0);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  m_quad.draw();
}

void FrameBufferViewport::drawInFrameBuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  // We need to draw the scene considering the viewport full size
  glViewport(0, 0, m_width, m_height);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto shader = m_shaderManager.getShader("cube");
  shader->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_metal.id());
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setInt("tex", 0);

  glm::mat4 model{1.f};
  model = glm::translate(model, glm::vec3(-1.f, 0.f, -1.f));
  shader->setMat4f("model", model);
  m_cube.draw();

  model = glm::translate({1.f}, glm::vec3(2.f, 0.f, 0.f));
  shader->setMat4f("model", model);
  m_cube.draw();
}