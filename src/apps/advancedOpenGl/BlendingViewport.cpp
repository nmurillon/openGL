#include "BlendingViewport.hpp"

#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <map>

BlendingViewport::BlendingViewport(const std::string &name, float width,
                                   float height)
    : libs::core::Viewport(name, width, height),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          ADVANCEDOPENGL_RESOURCES_FOLDER / "assets");

  const auto shaderDir = (assetsDir / "shaders").string();
  const auto textureDir = assetsDir / "textures";

  m_shaderManager.addShader("model",
                            std::format("{}/depthTesting.vert", shaderDir),
                            std::format("{}/depthTesting.frag", shaderDir));

  m_shaderManager.addShader("grass",
                            std::format("{}/blendingDiscard.vert", shaderDir),
                            std::format("{}/blendingDiscard.frag", shaderDir));
  // Setup textures
  m_marble = {libs::renderer::TextureType::DIFFUSE,
              std::format("{}/marble.jpg", textureDir.string())};

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};

  m_transparent = {
      libs::renderer::TextureType::DIFFUSE,
      std::format("{}/blending_transparent_window.png", textureDir.string())};

  m_transparent.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void BlendingViewport::initState() {
  m_openglStateCache->setActiveTexture(0);
  m_metal.bind();

  m_openglStateCache->setActiveTexture(1);
  m_marble.bind();

  m_openglStateCache->setActiveTexture(2);
  m_transparent.bind();

  m_openglStateCache->setDepthTest(true);
  m_openglStateCache->setBlend(true);
  m_openglStateCache->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BlendingViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
  m_openglStateCache->setBlend(false);

  // Unbind textures
  m_openglStateCache->setActiveTexture(2);
  m_transparent.unbind();

  m_openglStateCache->setActiveTexture(1);
  m_marble.unbind();

  m_openglStateCache->setActiveTexture(0);
  m_metal.unbind();
}

void BlendingViewport::drawScene() {
  m_openglStateCache->clear(GL_COLOR_BUFFER_BIT);

  ImGuiIO &io = ImGui::GetIO();
  const auto mousePos = io.MousePos;
  m_cameraController.setEnabled(isActive());

  m_camera->setViewportSize(m_width, m_height);
  m_cameraController.update();

  // floor
  drawFloor();

  // cubes
  drawCube(glm::vec3(-1.0f, 0.0f, -1.0f));
  drawCube(glm::vec3(2.0f, 0.0f, 0.0f));

  // grass
  drawTransparent();
}

void BlendingViewport::onImguiUpdate() {}

void BlendingViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void BlendingViewport::drawFloor() {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  shader->setMat4f("model", glm::mat4(1.0f));
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setInt("tex", 1);
  m_floor.draw();
}

void BlendingViewport::drawCube(const glm::vec3 &position) {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  shader->setInt("tex", 0);
  m_cube.draw();
}

void BlendingViewport::drawTransparent() {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  // Draw further objects first, else background objects will not be drawn
  std::map<float, glm::vec3> sorted;
  for (const auto &position : m_positions) {
    // Note that this works because all the distance are different at the start.
    float distance = glm::length(m_camera->getPosition() - position);
    sorted[distance] = position;
  }

  for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, it->second);

    shader->setMat4f("model", model);
    shader->setMat4f("projection", m_camera->getProjection());
    shader->setMat4f("view", m_camera->getViewMatrix());

    shader->setInt("tex", 2);
    m_transparentCube.draw();
  }
}