#include "ColorAppLayer.hpp"

#include "../../mesh.hpp"

#include <libs/renderer/Camera.hpp>
#include <libs/renderer/Shader.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

ColorAppLayer::ColorAppLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::Camera>(
                       glm::vec3(0.f, 0.0f, 5.0f))),
      m_shaderCube("shaders/basicShader.vert", "shaders/lightSource.frag"),
      m_shaderLight("shaders/basicShader.vert", "shaders/light.frag") {

  glGenVertexArrays(1, &m_vaoCube);
  glGenVertexArrays(1, &m_vaoLight);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  // Cube data
  glBindVertexArray(m_vaoCube);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(
                   mesh::lighting::verticesWithColors.size() * sizeof(float)),
               mesh::lighting::verticesWithColors.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(mesh::lighting::indicesForColors.size() *
                                       sizeof(unsigned int)),
               mesh::lighting::indicesForColors.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Light data
  glBindVertexArray(m_vaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Setup light shader data
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, glm::vec3(1.2f, 1.0f, 2.0f));
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  m_shaderLight.use();
  m_shaderLight.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
  m_shaderLight.setMat4f("model", lightModel);

  // Setup cube shader data
  m_shaderCube.setVec3f("lightColor", 1.f, 1.f, 1.f);
  m_shaderCube.setVec3f("objectColor", 1.f, 0.5f, 0.31f);
  m_shaderCube.setMat4f("model", glm::mat4(1.0f));
}

void ColorAppLayer::onUpdate() {
  // TODO: update aspect ratio with window resize
  glm::mat4 projection = glm::perspective(glm::radians(m_camera->getZoom()),
                                          800.f / 600.f, 0.1f, 100.f);

  m_shaderLight.setMat4f("view", m_camera->getViewMatrix());
  m_shaderLight.setMat4f("projection", projection);

  // Cube
  m_shaderCube.use();
  m_shaderCube.setMat4f("projection", projection);
  m_shaderCube.setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_vaoCube);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(mesh::lighting::verticesWithColors.size()),
      GL_UNSIGNED_INT, 0);

  // Light
  m_shaderLight.use();
  m_shaderLight.setMat4f("projection", projection);
  m_shaderLight.setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_vaoLight);
  glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(mesh::lighting::verticesWithColors.size()),
      GL_UNSIGNED_INT, 0);
}

void ColorAppLayer::onEvent(libs::events::Event &event) {
  // Handle events here
}