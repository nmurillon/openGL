#include "HousesViewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

HousesViewport::HousesViewport(const std::string &name, float width,
                               float height,
                               const std::filesystem::path &assetsDir)
    : Viewport(name, width, height), m_assetsDir(assetsDir) {
  // clang-format off
  float vertices[] = {
    // Position                 // Color
    -0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
    -0.5, 0.5, 0.0, 0.0, 1.0, 0.0,
    0.5, -0.5, 0.0, 0.0, 0.0, 1.0,
    0.5, 0.5, 0.0, 1.0, 1.0, 0.0
  };
  // clang-format on

  m_vao.setVertexBuffer(libs::renderer::VertexBuffer{
      libs::renderer::BufferLayout{
          libs::renderer::BufferLayoutElement{sizeof(float), 3},
          libs::renderer::BufferLayoutElement{sizeof(float), 3},
      },
      vertices, sizeof(vertices)});

  m_shaderManager.addShader("house", m_assetsDir / "house.vert",
                            m_assetsDir / "house.frag",
                            m_assetsDir / "house.geom");
}

void HousesViewport::initState() { m_openglStateCache->setDepthTest(true); }

void HousesViewport::resetState() { m_openglStateCache->setDepthTest(false); }

void HousesViewport::drawScene() {
  m_openglStateCache->setClearColor(0.1f, 0.1f, 0.1f, 0.1f);
  m_vao.bind();
  const auto shader = m_shaderManager.getShader("house");
  shader->use();

  glDrawArrays(GL_POINTS, 0, 4);
  shader->setVec3f("color", glm::vec3(0.f, 1.f, 0.f));

  m_vao.unbind();
}