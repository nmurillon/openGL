#include "CubemapViewport.hpp"

#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

CubemapViewport::CubemapViewport(const std::string &name, float width,
                                 float height,
                                 const std::filesystem::path &assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto cubemapDir = m_assetsDir / "textures" / "skybox";
  m_cubemap = libs::renderer::Cubemap{
      {cubemapDir / "right.jpg", cubemapDir / "left.jpg",
       cubemapDir / "top.jpg", cubemapDir / "bottom.jpg",
       cubemapDir / "front.jpg", cubemapDir / "back.jpg"}};

  m_shaderManager.addShader("cube", m_assetsDir / "shaders/cubemap/cube.vert",
                            m_assetsDir / "shaders/cubemap/cube.frag");

  m_shaderManager.addShader("cubemap",
                            m_assetsDir / "shaders/cubemap/cubemap.vert",
                            m_assetsDir / "shaders/cubemap/cubemap.frag");
}

void CubemapViewport::initState() {
  m_openglStateCache->setDepthTest(true);
  m_openglStateCache->setDepthFunc(GL_LEQUAL);
}

void CubemapViewport::resetState() {
  m_openglStateCache->setDepthTest(false);
  m_openglStateCache->setDepthFunc(GL_LESS);
}

void CubemapViewport::onImguiUpdate() {}

void CubemapViewport::onEvent(libs::events::Event &event) {
  if (isActive()) {
    m_cameraController.onEvent(event);
  }
}

void CubemapViewport::onViewportResize(float newWidth, float newHeight) {
  m_camera->setViewportSize(newWidth, newHeight);
}

void CubemapViewport::drawScene() {
  m_cameraController.update();
  m_openglStateCache->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw cube
  auto cubeShader = m_shaderManager.getShader("cube");
  cubeShader->use();

  m_openglStateCache->setActiveTexture(0);
  m_cubemap.bind();

  glm::mat4 model{1.f};
  model = glm::translate(model, glm::vec3(-1.f, 0.f, -1.f));

  cubeShader->setMat4f("model", model);
  cubeShader->setMat4f("view", m_camera->getViewMatrix());
  cubeShader->setMat4f("projection", m_camera->getProjection());
  cubeShader->setVec3f("camPos", m_camera->getPosition());
  cubeShader->setInt("cubemap", 0);
  m_cube.draw();

  // Draw cubemap
  auto cubemapShader = m_shaderManager.getShader("cubemap");
  cubemapShader->use();

  glm::mat4 view = glm::mat4(glm::mat3(m_camera->getViewMatrix()));
  cubemapShader->setMat4f("view", view);
  cubemapShader->setMat4f("projection", m_camera->getProjection());
  cubemapShader->setInt("cubemap", 0);

  m_openglStateCache->setDepthFunc(GL_LEQUAL);
  m_openglStateCache->setActiveTexture(0);
  m_cubemap.draw();
  m_openglStateCache->setDepthFunc(GL_LESS);
}