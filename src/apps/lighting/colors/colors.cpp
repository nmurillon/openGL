
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

#include <libs/core/Camera.hpp>
#include <libs/core/Shader.hpp>
#include <libs/core/Window.hpp>

#include "../mesh.hpp"

int main(int argc, char **argv) {
  libs::core::Window window{800, 600, "OpenGL - Colors chapter"};

  auto camera =
      std::make_shared<libs::core::Camera>(glm::vec3(0.f, 0.0f, 5.0f));

  camera->processKeyboardInput(libs::core::CameraMovement::RIGHT, 0.5);
  window.setUserData(&camera);
  window.setKeyboardInputCallback([](GLFWwindow *window, double deltaTime) {
    libs::core::CameraMovement cameraMovement;
    auto camera = *reinterpret_cast<std::shared_ptr<libs::core::Camera> *>(
        glfwGetWindowUserPointer(window));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      camera->processKeyboardInput(libs::core::CameraMovement::FORWARD,
                                   deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      camera->processKeyboardInput(libs::core::CameraMovement::BACKWARD,
                                   deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      camera->processKeyboardInput(libs::core::CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      camera->processKeyboardInput(libs::core::CameraMovement::RIGHT,
                                   deltaTime);
    }
  });

  window.setCursorPosCallback([](GLFWwindow *window, double xpos, double ypos) {
    static float lastX{0}, lastY{0};
    static bool firstMouse{true};
    auto camera = *reinterpret_cast<std::shared_ptr<libs::core::Camera> *>(
        glfwGetWindowUserPointer(window));

    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera->processMouseMovement(xOffset, yOffset);
  });

  window.setScrollCallback(
      [](GLFWwindow *window, double xoffset, double yOffset) {
        auto camera = *reinterpret_cast<std::shared_ptr<libs::core::Camera> *>(
            glfwGetWindowUserPointer(window));

        camera->processMouseScroll(yOffset);
      });

  // SETUP SHADERS
  const libs::core::Shader shaderCube("shaders/basicShader.vert",
                                      "shaders/lightSource.frag");

  const libs::core::Shader shaderLight("shaders/basicShader.vert",
                                       "shaders/light.frag");

  // VAO & VBO
  unsigned int VaoCube, VaoLight, VBO, EBO;
  glGenVertexArrays(1, &VaoCube);
  glGenVertexArrays(1, &VaoLight);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Cube data
  glBindVertexArray(VaoCube);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(
                   mesh::lighting::verticesWithColors.size() * sizeof(float)),
               mesh::lighting::verticesWithColors.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(mesh::lighting::indicesForColors.size() *
                                       sizeof(uint)),
               mesh::lighting::indicesForColors.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // Light data
  glBindVertexArray(VaoLight);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

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
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Camera
    projection = glm::perspective(glm::radians(camera->getZoom()),
                                  800.f / 600.f, 0.1f, 100.f);

    // Cube
    shaderCube.use();
    shaderCube.setVec3f("objectColor", 1.f, 0.5f, 0.31f);
    shaderCube.setVec3f("lightColor", 1.f, 1.f, 1.f);
    shaderCube.setMat4f("projection", projection);
    shaderCube.setMat4f("view", camera->getViewMatrix());
    shaderCube.setMat4f("model", glm::mat4(1.0f));

    glBindVertexArray(VaoCube);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(mesh::lighting::verticesWithColors.size()),
        GL_UNSIGNED_INT, 0);

    // Light
    shaderLight.use();
    shaderLight.setMat4f("projection", projection);
    shaderLight.setMat4f("view", camera->getViewMatrix());

    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, glm::vec3(1.2f, 1.0f, 2.0f));
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    shaderLight.setMat4f("model", lightModel);
    glBindVertexArray(VaoLight);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(mesh::lighting::verticesWithColors.size()),
        GL_UNSIGNED_INT, 0);
  });

  return 0;
}