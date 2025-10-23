
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <libs/core/Camera.hpp>
#include <libs/core/Shader.hpp>
#include <libs/core/Window.hpp>

#include "../mesh.hpp"

int main(int argc, char **argv) {
  libs::core::Window window{800, 600, "OpenGL - Colors chapter"};

  auto camera = std::make_shared<libs::core::Camera>(
      glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 1.0f, 0.0f), -80.f);

  // SETUP SHADERS
  const libs::core::Shader shaderLight("shaders/basicShader.vert",
                                       "shaders/lightSource.frag");

  // VAO & VBO
  std::vector<unsigned int> VAOs(2);
  unsigned int VBO, EBO;
  glGenVertexArrays(2, VAOs.data());
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Cube data
  for (std::size_t i = 0; i < VAOs.size(); ++i) {
    glBindVertexArray(VAOs.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(
                     mesh::lighting::verticesWithColors.size() * sizeof(float)),
                 mesh::lighting::verticesWithColors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(
                     mesh::lighting::indicesForColors.size() * sizeof(uint)),
                 mesh::lighting::indicesForColors.data(), GL_STATIC_DRAW);

    // Link Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);
  }

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  shaderLight.use();
  const std::vector<std::vector<float>> objectColors{{1.0f, 0.5f, 0.31f},
                                                     {1.0f, 1.0f, 1.0f}};
  shaderLight.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);

  // Create a transformation matrix
  glm::mat4 modelObject = glm::mat4(1.0f);
  glm::mat4 modelLight = glm::mat4(1.0f);
  modelLight = glm::translate(modelLight, glm::vec3(1.2f, 1.0f, 2.0f));
  modelLight = glm::scale(modelLight, glm::vec3(0.2f));

  glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()),
                                          800.f / 600.f, 0.1f, 100.f);

  std::vector<glm::mat4> models{modelObject, modelLight};

  shaderLight.setMat4f("view", camera->getViewMatrix());
  shaderLight.setMat4f("projection", projection);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  window.open([&]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::size_t i = 0; i < VAOs.size(); ++i) {
      glBindVertexArray(VAOs.at(i));
      shaderLight.setVec3f("objectColor", objectColors.at(i));

      shaderLight.setMat4f("model", models.at(i));

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glDrawElements(
          GL_TRIANGLES,
          static_cast<GLsizei>(mesh::lighting::verticesWithColors.size()),
          GL_UNSIGNED_INT, 0);
    }
  });

  return 0;
}