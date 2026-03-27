#include "DepthTestViewport.hpp"

#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

namespace {
int depthFuncFromString(const std::string &funcStr) {
  if (funcStr == "GL_NEVER")
    return GL_NEVER;
  if (funcStr == "GL_LESS")
    return GL_LESS;
  if (funcStr == "GL_EQUAL")
    return GL_EQUAL;
  if (funcStr == "GL_LEQUAL")
    return GL_LEQUAL;
  if (funcStr == "GL_GREATER")
    return GL_GREATER;
  if (funcStr == "GL_NOTEQUAL")
    return GL_NOTEQUAL;
  if (funcStr == "GL_GEQUAL")
    return GL_GEQUAL;
  if (funcStr == "GL_ALWAYS")
    return GL_ALWAYS;
  return GL_ALWAYS; // Default
}
} // namespace

DepthTestViewport::DepthTestViewport(const std::string &name, float width,
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

  m_shaderManager.addShader("modelOutline",
                            std::format("{}/depthTesting.vert", shaderDir),
                            std::format("{}/outline.frag", shaderDir));

  // Setup textures
  m_marble = {libs::renderer::TextureType::DIFFUSE,
              std::format("{}/marble.jpg", textureDir.string())};

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};
}

void DepthTestViewport::initState() {
  m_openglStateCache->setActiveTexture(0);
  m_metal.bind();

  m_openglStateCache->setActiveTexture(1);
  m_marble.bind();

  m_openglStateCache->setDepthTest(true);
}

void DepthTestViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
  m_openglStateCache->setStencilTest(false);

  // Unbind textures
  m_openglStateCache->setActiveTexture(0);
  m_metal.bind();
  m_openglStateCache->setActiveTexture(1);
  m_marble.unbind();
}

void DepthTestViewport::drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDepthFunc(m_depthFunc);

  if (m_showOutline) {
    m_openglStateCache->setStencilTest(true);
  }

  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  ImGuiIO &io = ImGui::GetIO();
  const auto mousePos = io.MousePos;
  m_cameraController.setEnabled(
      isActive() && isInViewport(mousePos.x, s_windowHeight - mousePos.y));

  m_camera->setViewportSize(m_width, m_height);
  m_cameraController.update();

  // floor
  drawFloor();

  // cubes
  drawCube(glm::vec3(-1.0f, 0.0f, -1.0f));
  drawCube(glm::vec3(2.0f, 0.0f, 0.0f));

  glBindVertexArray(0);
}

void DepthTestViewport::onImguiUpdate() {
  if (!isActive()) {
    return;
  }

  static std::string current_item = m_depthFuncs[0];

  ImGui::Begin("Depth Test Settings");

  ImGui::Checkbox("Show depth buffer", &m_showDepthBuffer);

  if (!m_showDepthBuffer &&
      ImGui::BeginCombo("Depth function", current_item.c_str())) {
    for (const auto &item : m_depthFuncs) {
      bool is_selected = (current_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        m_depthFunc = depthFuncFromString(item);
        current_item = item;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::SeparatorText("Outline settings");

  ImGui::Checkbox("Show outline", &m_showOutline);

  ImGui::ColorPicker3("Outline color", glm::value_ptr(m_outlineColor));
  ImGui::SliderFloat("Outline scale", &m_outlineScale, 1.0f, 1.5f);
  ImGui::Checkbox("Group outline", &m_showGroupedOutline);

  ImGui::End();
}

void DepthTestViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void DepthTestViewport::drawFloor() {
  // Don't update the stencil buffer
  glStencilMask(0x00);
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  shader->setInt("tex", 1);
  m_floor.draw();
}

void DepthTestViewport::drawCube(const glm::vec3 &position) {
  // All fragments pass the stencil test + Enable writing to the stencil buffer
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);

  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setBool("showDepthBuffer", m_showDepthBuffer);

  shader->setInt("tex", 0);
  m_cube.draw();

  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  m_openglStateCache->setDepthTest(false);

  if (m_showOutline) {

    auto outlineShader = m_shaderManager.getShader("modelOutline");
    outlineShader->use();

    model = glm::scale(model, glm::vec3(m_outlineScale));
    outlineShader->setInt("tex", 0);
    outlineShader->setMat4f("model", model);
    outlineShader->setMat4f("projection", m_camera->getProjection());
    outlineShader->setMat4f("view", m_camera->getViewMatrix());
    outlineShader->setVec3f("outlineColor", m_outlineColor);
    m_cube.draw();
  }

  // TODO: does this need to be in the if block?
  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glEnable(GL_DEPTH_TEST);

  if (!m_showGroupedOutline) {
    glClear(GL_STENCIL_BUFFER_BIT);
  }
}