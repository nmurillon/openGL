#include "FaceCullingViewport.hpp"

#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

const std::vector<std::string> cullModes{"GL_BACK", "GL_FRONT",
                                         "GL_FRONT_AND_BACK"};

const std::vector<std::string> frontFaceModes{"GL_CCW", "GL_CW"};

namespace {
int cullModeFromString(const std::string &cullMode) {
  if (cullMode == "GL_FRONT")
    return GL_FRONT;
  if (cullMode == "GL_BACK")
    return GL_BACK;
  if (cullMode == "GL_FRONT_AND_BACK")
    return GL_FRONT_AND_BACK;
  return GL_BACK; // Default
}

int frontFaceFromString(const std::string &frontFaceMode) {
  if (frontFaceMode == "GL_CCW")
    return GL_CCW;
  if (frontFaceMode == "GL_CW")
    return GL_CW;
  return GL_CCW; // Default
}
} // namespace

FaceCullingViewport::FaceCullingViewport(const std::string &name, float width,
                                         float height,
                                         const std::filesystem::path &assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (m_assetsDir / "shaders").string();
  const auto textureDir = m_assetsDir / "textures";

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};

  m_shaderManager.addShader("cube", std::format("{}/cube.vert", shaderDir),
                            std::format("{}/cube.frag", shaderDir));
}

void FaceCullingViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void FaceCullingViewport::initState() {
  m_openglStateCache->setFaceCulling(true);
  m_openglStateCache->setCullFace(m_cullFace);
  m_openglStateCache->setFrontFaceWinding(m_cullWinding);

  m_openglStateCache->setActiveTexture(0);
  m_metal.bind();
}

void FaceCullingViewport::resetState() {
  m_openglStateCache->setActiveTexture(0);
  m_metal.unbind();
  m_openglStateCache->setCullFace(GL_BACK);
  m_openglStateCache->setFrontFaceWinding(GL_CCW);
  m_openglStateCache->setFaceCulling(false);
}

void FaceCullingViewport::onImguiUpdate() {
  if (!isActive()) {
    return;
  }

  static std::string current_item = cullModes[0];

  ImGui::Begin("Face Culling Settings");

  if (ImGui::BeginCombo("Cull mode", current_item.c_str())) {
    for (const auto &item : cullModes) {
      bool is_selected = (current_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        m_cullFace = cullModeFromString(item);
        current_item = item;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  static std::string current_front_face_item = frontFaceModes[0];
  if (ImGui::BeginCombo("Cull front face", current_front_face_item.c_str())) {
    for (const auto &item : frontFaceModes) {
      bool is_selected = (current_front_face_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        m_cullWinding = frontFaceFromString(item);
        current_front_face_item = item;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::End();
}

void FaceCullingViewport::drawScene() {
  m_camera->setViewportSize(m_width, m_height);
  m_cameraController.update();

  auto shader = m_shaderManager.getShader("cube");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  shader->setInt("tex", 0);
  m_cube.draw();
}