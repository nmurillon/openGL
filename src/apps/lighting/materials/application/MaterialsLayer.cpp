#include "MaterialsLayer.hpp"

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
#include <format>

unsigned int loadTexture(const std::string &textureFile,
                         GLenum textureUnit = GL_TEXTURE0,
                         GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  const std::string texturePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       MATERIALS_RESOURCES_FOLDER / textureFile)
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

MaterialsLayer::MaterialsLayer(const std::string &name)
    : Layer(name), m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
                       glm::vec3(0.f, 0.0f, 5.0f))),
      m_cameraController(m_camera) {

  const auto shaderDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          MATERIALS_RESOURCES_FOLDER / "shaders")
                             .string();

  m_shaderManager.addShader("cube", std::format("{}/materials.vert", shaderDir),
                            std::format("{}/materials.frag", shaderDir));

  m_shaderManager.addShader("cubeMatMaps",
                            std::format("{}/materialsMap.vert", shaderDir),
                            std::format("{}/materialsMap.frag", shaderDir));

  m_shaderManager.addShader(
      "light",
      (m_shaderManager.getCommonShaderDirectory() / "basicShader.vert")
          .string(),
      std::format("{}/light.frag", shaderDir));

  // Setup textures
  m_diffuseMap = loadTexture("assets/wood_container.png", GL_TEXTURE0);
  m_specularMap =
      loadTexture("assets/wood_container_specular.png", GL_TEXTURE1);
  m_emissionMap = loadTexture("assets/matrix.jpg", GL_TEXTURE2);

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

void MaterialsLayer::onUpdate() {
  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  m_cameraController.update();

  m_light.position = glm::vec3(1.2f * cos(currentFrame), sin(currentFrame),
                               2.0f * cos(currentFrame));

  // Cube
  updateShaderCube();

  glBindVertexArray(m_vaoCube);
  glDrawElements(GL_TRIANGLES,
                 static_cast<GLsizei>(
                     mesh::lighting::withNormalsAndTexCoords::vertices.size()),
                 GL_UNSIGNED_INT, 0);

  // Light
  updateShaderLight();

  glBindVertexArray(m_vaoLight);
  glDrawElements(GL_TRIANGLES,
                 static_cast<GLsizei>(
                     mesh::lighting::withNormalsAndTexCoords::vertices.size()),
                 GL_UNSIGNED_INT, 0);
}

void MaterialsLayer::onImguiUpdate() {
  ImGui::Begin("Properties");

  ImGui::Checkbox("Use lighting maps", &m_useLightingMaps);

  if (!m_useLightingMaps) {
    ImGui::SeparatorText("Material properties");

    auto materials = getAllMaterialTypeNames();
    static std::string current_item = getAllMaterialTypeNames()[0];

    if (ImGui::BeginCombo("Material type", current_item.c_str())) {
      for (const auto &item : materials) {
        bool is_selected = (current_item == item);
        if (ImGui::Selectable(item.c_str(), is_selected)) {
          current_item = item;
          m_material = getMaterialByString(item);
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    ImGui::SliderFloat3("Material Ambient", glm::value_ptr(m_material.ambient),
                        0.0f, 1.0f);
    ImGui::SliderFloat3("Material Diffuse", glm::value_ptr(m_material.diffuse),
                        0.0f, 1.0f);
    ImGui::SliderFloat3("Material Specular",
                        glm::value_ptr(m_material.specular), 0.0f, 1.0f);
    ImGui::SliderFloat("Material Shininess", &m_material.shininess, 0.0f,
                       256.0f);

    if (ImGui::Button("Reset material")) {
      m_material = tutorial;
    }
  } else {
    ImGui::Checkbox("Show emission maps", &m_showEmissionMap);
  }

  ImGui::SeparatorText("Light properties");

  ImGui::ColorEdit3("Light Color", glm::value_ptr(m_light.color));
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

void MaterialsLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(MaterialsLayer::onWindowResized));

  m_cameraController.onEvent(event);
}

bool MaterialsLayer::onWindowResized(libs::events::WindowResizeEvent &event) {
  float width = static_cast<float>(event.getWidth());
  float height = static_cast<float>(event.getHeight());

  m_aspectRatio = width / height;

  m_camera->setViewportSize(width, height);
  return event.handle();
}

void MaterialsLayer::updateShaderCube() {
  auto projection = m_camera->getProjection();
  auto view = m_camera->getViewMatrix();

  const auto shaderName = m_useLightingMaps ? "cubeMatMaps" : "cube";

  auto shader = m_shaderManager.getShader(shaderName);

  shader->use();
  shader->setMat4f("model", glm::mat4(1.0f));
  shader->setMat4f("view", view);
  shader->setMat4f("projection", projection);

  if (m_useLightingMaps) {
    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    shader->setInt("material.emissionMap", 2);
    shader->setFloat("useEmissionMap", m_showEmissionMap ? 1.0 : 0.f);
  } else {
    shader->setVec3f("material.ambient", m_material.ambient);
    shader->setVec3f("material.diffuse", m_material.diffuse);
    shader->setVec3f("material.specular", m_material.specular);
  }

  shader->setFloat("material.shininess", m_material.shininess);

  shader->setVec3f("light.color", m_light.color);
  shader->setVec3f("light.position", m_light.position);
  shader->setVec3f("light.ambient", m_light.ambient);
  shader->setVec3f("light.diffuse", m_light.diffuse);
  shader->setVec3f("light.specular", m_light.specular);

  shader->setVec3f("viewPos", m_camera->getPosition());
}

void MaterialsLayer::updateShaderLight() {
  auto shader = m_shaderManager.getShader("light");

  shader->use();
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, m_light.position);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  shader->setVec3f("lightColor", m_light.color);
  shader->setMat4f("model", lightModel);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
}