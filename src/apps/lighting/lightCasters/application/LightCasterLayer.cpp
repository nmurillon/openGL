#include "LightCasterLayer.hpp"

#include "../../mesh.hpp"

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
       LIGHTCASTER_RESOURCES_FOLDER / textureFile)
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

LightCasterLayer::LightCasterLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
                       glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          LIGHTCASTER_RESOURCES_FOLDER / "shaders")
                             .string();

  m_shaderManager.addShader("Directional Light",
                            std::format("{}/directionalLight.vert", shaderDir),
                            std::format("{}/directionalLight.frag", shaderDir));

  m_shaderManager.addShader("Point Light",
                            std::format("{}/pointLight.vert", shaderDir),
                            std::format("{}/pointLight.frag", shaderDir));

  m_shaderManager.addShader(
      "lightSource",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

  // Setup textures
  m_diffuseMap = loadTexture("assets/wood_container.png", GL_TEXTURE0);
  m_specularMap =
      loadTexture("assets/wood_container_specular.png", GL_TEXTURE1);

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

void LightCasterLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  m_cameraController.update();

  // Cube
  updateShaderCube();

  // Light
  if (m_currentLightType == "Point Light") {
    updateShaderLight();
  }
}

void LightCasterLayer::onImguiUpdate() {
  ImGui::Begin("Light Properties");

  static std::string current_item = lightTypes[0];

  if (ImGui::BeginCombo("Light type", m_currentLightType.c_str())) {
    for (const auto &item : lightTypes) {
      bool is_selected = (current_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        m_currentLightType = item;
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  if (m_currentLightType == "Directional Light") {
    ImGui::SliderFloat3("Light Direction", glm::value_ptr(m_light.direction),
                        -500.0f, 500.0f);
  } else if (m_currentLightType == "Point Light") {
    ImGui::SliderFloat3("Light Position", glm::value_ptr(m_light.position),
                        -10.0f, 10.0f);

    ImGui::SliderFloat("Constant", &m_light.constant, 0.0f, 2.0f);
    ImGui::SliderFloat("Linear", &m_light.linear, 0.0f, 0.5f);
    ImGui::SliderFloat("Quadratic", &m_light.quadratic, 0.0f, 0.1f);
  }
  // else if (m_currentLightType == "Spotlight") {
  //   ImGui::SliderFloat3("Light Position", glm::value_ptr(m_light.position),
  //                       -10.0f, 10.0f);
  //   ImGui::SliderFloat3("Light Direction", glm::value_ptr(m_light.direction),
  //                       -500.0f, 500.0f);

  // }

  ImGui::SliderFloat3("Light Ambient", glm::value_ptr(m_light.ambient), 0.0f,
                      1.0f);
  ImGui::SliderFloat3("Light Diffuse", glm::value_ptr(m_light.diffuse), 0.0f,
                      1.0f);
  ImGui::SliderFloat3("Light Specular", glm::value_ptr(m_light.specular), 0.0f,
                      1.0f);

  if (ImGui::Button("Reset light")) {
    m_light = defaultLight;
  }

  ImGui::ShowDemoWindow();
  ImGui::End();
}

void LightCasterLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(LightCasterLayer::onWindowResized));

  m_cameraController.onEvent(event);
}

bool LightCasterLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  float width = static_cast<float>(event.getWidth());
  float height = static_cast<float>(event.getHeight());

  m_aspectRatio = width / height;

  m_camera->setViewportSize(width, height);
  return event.handle();
}

void LightCasterLayer::updateShaderCube() {
  auto projection = m_camera->getProjection();
  auto view = m_camera->getViewMatrix();

  auto shader = m_shaderManager.getShader(m_currentLightType);

  shader->use();

  shader->setInt("material.diffuse", 0);
  shader->setInt("material.specular", 1);

  shader->setMat4f("view", view);
  shader->setMat4f("projection", projection);

  shader->setFloat("material.shininess", 32.f);

  if (m_currentLightType == "Directional Light") {
    shader->setVec3f("light.direction", m_light.direction);
  } else if (m_currentLightType == "Point Light") {
    shader->setVec3f("light.position", m_light.position);
    shader->setFloat("light.constant", m_light.constant);
    shader->setFloat("light.linear", m_light.linear);
    shader->setFloat("light.quadratic", m_light.quadratic);
  }

  shader->setVec3f("light.ambient", m_light.ambient);
  shader->setVec3f("light.diffuse", m_light.diffuse);
  shader->setVec3f("light.specular", m_light.specular);

  shader->setVec3f("viewPos", m_camera->getPosition());

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

void LightCasterLayer::updateShaderLight() {
  auto shader = m_shaderManager.getShader("lightSource");

  shader->use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, m_light.position);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  shader->setVec3f("lightColor", m_light.color);
  shader->setMat4f("model", lightModel);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_vaoLight);
  glDrawElements(GL_TRIANGLES,
                 static_cast<GLsizei>(
                     mesh::lighting::withNormalsAndTexCoords::vertices.size()),
                 GL_UNSIGNED_INT, 0);
}