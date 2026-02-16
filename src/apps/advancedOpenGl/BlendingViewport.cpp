#include "BlendingViewport.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

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

  // Setup textures

  m_marble = {libs::renderer::TextureType::DIFFUSE,
              std::format("{}/marble.jpg", textureDir.string())};

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};

  m_grass = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/grass.png", textureDir.string())};

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_metal.id());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_marble.id());

  glEnable(GL_DEPTH_TEST);
}

void BlendingViewport::drawScene() {
  glClear(GL_COLOR_BUFFER_BIT);

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

  glBindVertexArray(0);
}

void BlendingViewport::onImguiUpdate() {}

void BlendingViewport::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  m_cameraController.onEvent(event);
}

void BlendingViewport::drawFloor() {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

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