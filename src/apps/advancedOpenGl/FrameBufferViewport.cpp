#include "FrameBufferViewport.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

#include <libs/logger/Logger.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

const std::map<std::string, std::vector<float>> FrameBufferViewport::s_kernels{
    {"custom", {0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f}},
    {"edgeDetection", {1.f, 1.f, 1.f, 1.f, -8.f, 1.f, 1.f, 1.f, 1.f}},
    {"sharpen", {2.f, 2.f, 2.f, 2.f, -15.f, 2.f, 2.f, 2.f, 2.f}},
    {"blur",
     {1.f / 16.f, 2.f / 16.f, 1.f / 16.f, 2.f / 16.f, 4.f / 16.f, 2.f / 16.f,
      1.f / 16.f, 2.f / 16.f, 1.f / 16.f}}};

FrameBufferViewport::FrameBufferViewport(const std::string &name, float width,
                                         float height,
                                         const std::filesystem::path &assetsDir)
    : Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera), m_frameBuffer(width, height) {

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

  m_shaderManager.addShader("kernel", m_assetsDir / "shaders/frameBuffer.vert",
                            m_assetsDir / "shaders/kernel.frag");

  m_wood = {libs::renderer::TextureType::DIFFUSE,
            std::format("{}/wood_container.png",
                        (m_assetsDir / "textures").string())};
}

void FrameBufferViewport::initState() {
  // m_openglStateCache->setActiveTexture(0);
  // m_textureColorBuffer.bind();
  m_openglStateCache->setDepthTest(true);
}

void FrameBufferViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Unbind textures
  // m_openglStateCache->setActiveTexture(0);
  // m_textureColorBuffer.unbind();
}

void FrameBufferViewport::onImguiUpdate() {
  if (!isActive()) {
    return;
  }

  static std::vector<std::string> postProcessingEffects{
      "framebuffer", "grayscale", "inversion", "kernel"};
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

  static std::string kernelOption = "custom";

  if (current_item == "kernel") {
    if (ImGui::BeginCombo("Kernel", kernelOption.c_str())) {
      for (const auto &[key, value] : s_kernels) {
        bool is_selected = (current_item == key);
        if (ImGui::Selectable(key.c_str(), is_selected)) {
          m_kernel = value;
          kernelOption = key;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    int i = 0;
    for (auto &factor : m_kernel) {
      ImGui::SliderFloat(std::format("##kernel {}", i++).c_str(), &factor,
                         -10.f, 10.f);
    }
  }

  ImGui::End();
}

void FrameBufferViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void FrameBufferViewport::onViewportResize(float newWidth, float newHeight) {
  m_frameBuffer.setSize(newWidth, newHeight);
  m_camera->setViewportSize(newWidth, newHeight);
}

void FrameBufferViewport::drawScene() {
  m_cameraController.update();

  m_openglStateCache->setActiveTexture(0);
  drawInFrameBuffer();

  m_openglStateCache->setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  m_openglStateCache->setViewport(m_xBottomLeft, m_yBottomLeft, m_width,
                                  m_height);
  m_openglStateCache->setDepthTest(false);
  m_openglStateCache->clear(GL_COLOR_BUFFER_BIT);

  auto screenShader = m_shaderManager.getShader(m_currentShader);
  screenShader->use();

  m_frameBuffer.getColorBuffer().bind();
  screenShader->setInt("screenTexture", 0);

  if (m_currentShader == "kernel") {
    for (int i = 0; i < 9; ++i) {
      screenShader->setFloat(std::format("kernel[{}]", i), m_kernel[i]);
    }
  }

  m_quad.draw();

  m_openglStateCache->setActiveTexture(0);
  drawRearView();

  m_openglStateCache->setViewport(m_xBottomLeft, m_yBottomLeft, m_width,
                                  m_height);
  m_openglStateCache->setDepthTest(false);

  screenShader->use();

  m_frameBuffer.getColorBuffer().bind();
  screenShader->setInt("screenTexture", 0);
  m_quadMirror.draw();
}

void FrameBufferViewport::drawInFrameBuffer() {
  m_frameBuffer.bind();
  // We need to draw the scene considering the viewport full size
  m_openglStateCache->setViewport(0, 0, m_width, m_height);

  m_openglStateCache->setDepthTest(true);
  m_openglStateCache->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  m_openglStateCache->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto shader = m_shaderManager.getShader("cube");
  shader->use();

  m_wood.bind();
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

  m_frameBuffer.unbind();
}

void FrameBufferViewport::drawRearView() {
  m_frameBuffer.bind();
  // We need to draw the scene considering the viewport full size
  m_openglStateCache->setViewport(0, 0, m_width, m_height);

  m_openglStateCache->setDepthTest(true);
  m_openglStateCache->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  m_openglStateCache->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto shader = m_shaderManager.getShader("cube");
  shader->use();

  m_wood.bind();
  m_camera->rotate(180.f, 0.f);
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

  m_camera->rotate(-180.f, 0.f);
  m_frameBuffer.unbind();
}