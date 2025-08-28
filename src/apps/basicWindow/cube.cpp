#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <libs/core/Shader.hpp>
#include <libs/io/ProgramPath.hpp>
#include <stb_image/stb_image.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

float textureMixingOpacity = 0.5f;

// Callback to resize the window if user resizes it
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Handle user inputs to close window if escaped is pressed
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    textureMixingOpacity = std::min(1.0f, textureMixingOpacity + 0.1f);
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    textureMixingOpacity = std::max(0.f, textureMixingOpacity - 0.1f);
  }
}

uint loadTexture(const std::string &textureFile,
                 GLenum textureUnit = GL_TEXTURE0, GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  const std::string texturePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       CUBE_RESOURCES_FOLDER / textureFile)
          .string()};

  unsigned char *data =
      stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);

  uint texture;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "WARNING::TEXTURE::FAIL_TO_OPEN" << std::endl;
  }

  stbi_image_free(data);

  return texture;
}

int main(int argc, char **argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // SETUP SHADERS
  const libs::core::Shader shaderProgram("shaders/basicShader.vert",
                                         "shaders/basicShader.frag");

  // SETUP VERTEX DATA
  // clang-format off

  // Position as a vec3 and color as a vec3 and texture as vec2
  std::vector<float> vertices = {
  -0.5f,    0.5f,  0.5f,  1.f,  1.f,  0.f,  0.f,  1.f, // TOP-LEFT-UP
   0.5f,    0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f, 1.f, // TOP-RIGHT-UP
  -0.5f, -0.5f, 0.5f, 1.f, 1.f, 0.f, 0.f, 0.f, // BOTTOM-LEFT-UP
  0.5f,  -0.5f, 0.5f, 1.f, 1.f, 0.f, 1.f, 0.f,// BOTTOM-RIGHT-UP

  -0.5f,  0.5f, -0.5f, 1.f, 1.f, 0.f, 0.f,  1.f,// TOP-LEFT-DOWN
   0.5f,  0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f, 1.f,// TOP-RIGHT-DOWN
  -0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 0.f, 0.f,// BOTTOM-LEFT-DOWN
   0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f, 0.f,// BOTTOM-RIGHT-DOWN
  };

  std::vector<uint> indices = {
      // Front face
      0,1,2,
      1,2,3,
      // Left face
      0,2,6,
      0,4,6,
      // Up face
      0,1,5,
      0,4,5,
      // Right face
      1,3,5,
      3,5,7,
      // Down face
      2,3,6,
      3,6,7,
      // Background face
      4,5,6,
      5,6,7
  };
  // clang-format on

  // VAO & VBO
  uint VAO, EBO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  loadTexture("textures/totk.jpg", GL_TEXTURE0, GL_CLAMP_TO_EDGE);
  loadTexture("textures/kaamelott.jpg", GL_TEXTURE1);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(indices.size() * sizeof(uint)),
               indices.data(), GL_STATIC_DRAW);

  // Link Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        reinterpret_cast<void *>(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glViewport(0, 0, 800, 600);

  // To draw in wireframe mode, use GL_LINE
  std::vector<GLenum> modes{GL_LINE, GL_FILL};

  shaderProgram.use();
  shaderProgram.setInt("Texture1", 0);
  shaderProgram.setInt("Texture2", 1);

  // Create a transformation matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.f, 0.f, -3.0f));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

  // In real case: the following will be done in the while loop as the different
  // matrices can change
  shaderProgram.setMat4f("model", model);
  shaderProgram.setMat4f("view", view);
  shaderProgram.setMat4f("projection", projection);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Wait for user input to keep the window opened
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    shaderProgram.setFloat("TextureMixingOpacity", textureMixingOpacity);
    model = glm::mat4(1.0f);
    model = glm::rotate(model,
                        static_cast<float>(glfwGetTime() * glm::radians(45.0f)),
                        glm::vec3(0.5f, 1.0f, 0.0f));
    shaderProgram.setMat4f("model", model);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < 2; ++i) {
      glPolygonMode(GL_FRONT_AND_BACK, modes.at(i));
      glBindVertexArray(VAO);

      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                     GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean data before exiting
  glfwTerminate();

  return 0;
}