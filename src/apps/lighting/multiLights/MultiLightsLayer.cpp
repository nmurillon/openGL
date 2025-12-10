#include "MultiLightsLayer.hpp"

#include "../mesh.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <iostream>
#include <stb_image/stb_image.h>

#include <cmath>

unsigned int loadTexture(const std::string &textureFile,
                         GLenum textureUnit = GL_TEXTURE0,
                         GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  const std::string texturePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       MULTILIGHTS_RESOURCES_FOLDER / textureFile)
          .string()};

  unsigned char *data =
      stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);

  // Bind the texture to work on it
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if (data) {

    GLenum format;
    if (nChannels == 1)
      format = GL_RED;
    else if (nChannels == 3)
      format = GL_RGB;
    else if (nChannels == 4)
      format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "WARNING::TEXTURE::FAIL_TO_OPEN" << std::endl;
  }

  stbi_image_free(data);

  return texture;
}

MultiLightsLayer::MultiLightsLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
                       glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          MULTILIGHTS_RESOURCES_FOLDER / "assets" / "shaders")
                             .string();

  m_shaderManager.addShader("cube",
                            std::format("{}/multiLight.vert", shaderDir),
                            std::format("{}/multiLight.frag", shaderDir));

  m_shaderManager.addShader(
      "lightSource",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

  // Setup textures
  m_diffuseMap = loadTexture("assets/textures/wood_container.png", GL_TEXTURE0);
  m_specularMap =
      loadTexture("assets/textures/wood_container_specular.png", GL_TEXTURE1);

  glGenVertexArrays(1, &m_vaoCube);
  glGenVertexArrays(1, &m_vaoLight);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  // Cube data
  glBindVertexArray(m_vaoCube);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(
                   mesh::lighting::withNormalsAndTexCoords::vertices.size() *
                   sizeof(float)),
               mesh::lighting::withNormalsAndTexCoords::vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(
                   mesh::lighting::withNormalsAndTexCoords::indices.size() *
                   sizeof(unsigned int)),
               mesh::lighting::withNormalsAndTexCoords::indices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(6 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Light data
  glBindVertexArray(m_vaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);
}

void MultiLightsLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  m_cameraController.update();

  // Cube
  updateCubes();

  // Lights
  updateLights();
}

void MultiLightsLayer::onImguiUpdate() {
  ImGui::Begin("Light Properties");

  ImGui::SeparatorText("Directional Light Settings");
  ImGui::SliderFloat3("Directional Light Direction",
                      glm::value_ptr(m_directionalLight.direction), -30.0f,
                      30.0f);

  ImGui::SliderFloat3("Directional Light Ambient",
                      glm::value_ptr(m_directionalLight.ambient), 0.0f, 1.0f);
  ImGui::SliderFloat3("Directional Light Diffuse",
                      glm::value_ptr(m_directionalLight.diffuse), 0.0f, 1.0f);
  ImGui::SliderFloat3("Directional Light Specular",
                      glm::value_ptr(m_directionalLight.specular), 0.0f, 1.0f);

  if (ImGui::Button("Reset directional light")) {
    m_directionalLight = s_defaultDirectionalLight;
  }

  // TODO: add button to dynamically add point lights up to a max number
  for (std::size_t i = 0; i < m_pointLights.size(); ++i) {
    ImGui::SeparatorText(std::format("Point Light {}", i + 1).c_str());

    ImGui::SliderFloat3(std::format("Point Light {} Position", i + 1).c_str(),
                        glm::value_ptr(m_pointLights[i].position), -10.0f,
                        10.0f);

    ImGui::SliderFloat(std::format("Point Light {} Constant", i + 1).c_str(),
                       &m_pointLights[i].constant, 0.0f, 2.0f);

    ImGui::SliderFloat(std::format("Point Light {} Linear", i + 1).c_str(),
                       &m_pointLights[i].linear, 0.0f, 0.5f);
    ImGui::SliderFloat(std::format("Point Light {} Quadratic", i + 1).c_str(),
                       &m_pointLights[i].quadratic, 0.0f, 0.1f);

    ImGui::SliderFloat3(std::format("Point Light {} Ambient", i + 1).c_str(),
                        glm::value_ptr(m_pointLights[i].ambient), 0.0f, 1.0f);
    ImGui::SliderFloat3(std::format("Point Light {} Diffuse", i + 1).c_str(),
                        glm::value_ptr(m_pointLights[i].diffuse), 0.0f, 1.0f);
    ImGui::SliderFloat3(std::format("Point Light {} Specular", i + 1).c_str(),
                        glm::value_ptr(m_pointLights[i].specular), 0.0f, 1.0f);

    if (ImGui::Button(std::format("Reset Point Light {}", i + 1).c_str())) {
      m_pointLights[i] = s_defaultPointLight;
      m_pointLights[i].position = mesh::lighting::pointLightPositions[i];
    }
  }

  ImGui::ShowDemoWindow();
  ImGui::End();
}

void MultiLightsLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(MultiLightsLayer::onWindowResized));

  m_cameraController.onEvent(event);
}

bool MultiLightsLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  float width = static_cast<float>(event.getWidth());
  float height = static_cast<float>(event.getHeight());

  m_aspectRatio = width / height;

  m_camera->setViewportSize(width, height);
  return event.handle();
}

void MultiLightsLayer::updateCubes() {
  auto projection = m_camera->getProjection();
  auto view = m_camera->getViewMatrix();

  auto shader = m_shaderManager.getShader("cube");

  shader->use();

  shader->setInt("material.diffuse", 0);
  shader->setInt("material.specular", 1);

  shader->setMat4f("view", view);
  shader->setMat4f("projection", projection);
  shader->setVec3f("viewPos", m_camera->getPosition());

  shader->setFloat("material.shininess", 32.f);

  // Set directional Light
  shader->setVec3f("directionalLight.direction", m_directionalLight.direction);
  shader->setVec3f("directionalLight.ambient", m_directionalLight.ambient);
  shader->setVec3f("directionalLight.diffuse", m_directionalLight.diffuse);
  shader->setVec3f("directionalLight.specular", m_directionalLight.specular);

  // Set point lights
  for (std::size_t i = 0; i < m_pointLights.size(); ++i) {
    const auto &pointLight = m_pointLights[i];
    std::string baseName = std::format("pointLights[{}].", i);

    shader->setVec3f(baseName + "position", pointLight.position);
    shader->setFloat(baseName + "constant", pointLight.constant);
    shader->setFloat(baseName + "linear", pointLight.linear);
    shader->setFloat(baseName + "quadratic", pointLight.quadratic);
    shader->setVec3f(baseName + "ambient", pointLight.ambient);
    shader->setVec3f(baseName + "diffuse", pointLight.diffuse);
    shader->setVec3f(baseName + "specular", pointLight.specular);
  }

  glBindVertexArray(m_vaoCube);

  for (int i = 0; i < 10; ++i) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(mesh::lighting::cubePositions[i]));
    model = glm::rotate(model, glm::radians(20.0f * i),
                        glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4f("model", model);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(
            mesh::lighting::withNormalsAndTexCoords::vertices.size()),
        GL_UNSIGNED_INT, 0);
  }
}

void MultiLightsLayer::updateLights() {
  auto shader = m_shaderManager.getShader("lightSource");

  shader->use();

  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setMat4f("projection", m_camera->getProjection());

  for (std::size_t i = 0; i < m_pointLights.size(); ++i) {
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel =
        glm::translate(lightModel, mesh::lighting::pointLightPositions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    shader->setVec3f("lightColor", m_pointLights[i].color);

    shader->setMat4f("model", lightModel);

    glBindVertexArray(m_vaoLight);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(
            mesh::lighting::withNormalsAndTexCoords::vertices.size()),
        GL_UNSIGNED_INT, 0);
  }
}