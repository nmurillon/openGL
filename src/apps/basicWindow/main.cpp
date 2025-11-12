#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/ShaderManager.hpp>
#include <stb_image/stb_image.h>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

const std::vector<std::vector<float>> colors{
    {1.f, 0.5f, 0.2f, 1.f}, // Orange
    {1.f, 1.f, 0.f, 1.f}    // Yellow
};

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

unsigned int loadTexture(const std::string &textureFile,
                         GLenum textureUnit = GL_TEXTURE0,
                         GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  const std::string texturePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       BASIC_WINDOW_RESOURCES_FOLDER / textureFile)
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
  libs::renderer::ShaderManager shaderManager{};
  const auto shader = shaderManager.getShader("loglBasicShader");

  // SETUP VERTEX DATA
  // clang-format off

  // Position as a vec3 and color as a vec3 and texture as vec2
  std::vector<std::vector<float>> vertices = {{-0.5f, -0.5f, 0.f, 1.0f, 0.f, 0.f, 0.f, 0.f,
                                                   0.f, 0.5f, 0.f, 0.0f, 1.f, 0.f, 0.5f, 0.5f,
                                                   0.5f, -0.5f, 0.f, 0.0f, 0.f, 1.f, 1.f, 0.f},
                                               {-1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
                                                    0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.5f*1.f, 0.f,
                                                    -0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.5f*0.5f, 0.5f*1.f,
                                                    // Second triangle
                                                    1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
                                                    0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.5f*0.5f, 0.5f*1.f}};


std::vector<std::vector<unsigned int>> indices = {
    {0,1,2},
    {0,1,2,
        1,3,4}
};
  // clang-format on

  // VAO & VBO
  std::vector<unsigned int> VAO(2), EBO(2), VBO(2);
  glGenVertexArrays(2, VAO.data());
  glGenBuffers(2, VBO.data());
  glGenBuffers(2, EBO.data());

  loadTexture("textures/totk.jpg", GL_TEXTURE0, GL_CLAMP_TO_EDGE);
  loadTexture("textures/kaamelott.jpg", GL_TEXTURE1);
  for (size_t i = 0; i < 2; ++i) {
    glBindVertexArray(VAO.at(i));

    glBindBuffer(GL_ARRAY_BUFFER, VBO.at(i));
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.at(i).size() * sizeof(float)),
                 vertices.at(i).data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO.at(i));
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.at(i).size() * sizeof(unsigned int)),
        indices.at(i).data(), GL_STATIC_DRAW);

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
  }

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glViewport(0, 0, 800, 600);

  // To draw in wireframe mode, use GL_LINE
  std::vector<GLenum> modes{GL_LINE, GL_FILL};

  shader->use();
  shader->setInt("Texture1", 0);
  shader->setInt("Texture2", 1);

  // Create a transformation matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
  // transformation = glm::scale(transformation, glm::vec3(0.25f, 0.25f, 0.f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.f, 0.f, -3.0f));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

  // In real case: the following will be done in the while loop as the different
  // matrices can change
  shader->setMat4f("model", model);
  shader->setMat4f("view", view);
  shader->setMat4f("projection", projection);

  // Wait for user input to keep the window opened
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    shader->setFloat("TextureMixingOpacity", textureMixingOpacity);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < 2; ++i) {
      glPolygonMode(GL_FRONT_AND_BACK, modes.at(i));
      glBindVertexArray(VAO.at(i));
      auto color = colors.at(i);

      shader->setVec4f("vertexColor", color);
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.at(i).size()),
                     GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean data before exiting
  glfwTerminate();

  return 0;
}