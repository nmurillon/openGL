#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <libs/core/Shader.hpp>
#include <libs/io/ProgramPath.hpp>
#include <stb_image/stb_image.h>
#include <vector>

const std::vector<std::vector<float>> colors{
    {1.f, 0.5f, 0.2f, 1.f}, // Orange
    {1.f, 1.f, 0.f, 1.f}    // Yellow
};

// Callback to resize the window if user resizes it
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Handle user inputs to close window if escaped is pressed
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

uint loadTexture(const std::string &textureFile,
                 GLenum textureUnit = GL_TEXTURE0) {
  int width, height, nChannels;
  const std::string texturePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       BASIC_WINDOW_RESOURCES_FOLDER / textureFile)
          .string()};

  unsigned char *data =
      stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);

  uint texture;
  glGenTextures(1, &texture);

  // Bind the texture to work on it
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
  std::vector<std::vector<float>> vertices = {{-0.5f, -0.5f, 0.f, 1.0f, 0.f, 0.f, 0.f, 0.f,
                                                   0.f, 0.5f, 0.f, 0.0f, 1.f, 0.f, 0.5f, 0.5f,
                                                   0.5f, -0.5f, 0.f, 0.0f, 0.f, 1.f, 1.f, 0.f},
                                               {-1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
                                                    0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f,
                                                    -0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.5f, 1.f,
                                                    // Second triangle
                                                    1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
                                                    0.5f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.5f, 1.f}};


std::vector<std::vector<uint>> indices = {
    {0,1,2},
    {0,1,2,
        1,3,4}
};
  // clang-format on

  // VAO & VBO
  std::vector<uint> VAO(2), EBO(2), VBO(2);
  glGenVertexArrays(2, VAO.data());
  glGenBuffers(2, VBO.data());
  glGenBuffers(2, EBO.data());

  loadTexture("textures/totk.jpg");
  loadTexture("textures/kaamelott.jpg", GL_TEXTURE1);
  for (size_t i = 0; i < 2; ++i) {
    glBindVertexArray(VAO.at(i));

    glBindBuffer(GL_ARRAY_BUFFER, VBO.at(i));
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.at(i).size() * sizeof(float)),
                 vertices.at(i).data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO.at(i));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.at(i).size() * sizeof(uint)),
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

  shaderProgram.use();
  shaderProgram.setInt("Texture1", 0);
  shaderProgram.setInt("Texture2", 1);

  // Wait for user input to keep the window opened
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (size_t i = 0; i < 2; ++i) {
      glPolygonMode(GL_FRONT_AND_BACK, modes.at(i));
      glBindVertexArray(VAO.at(i));
      auto color = colors.at(i);

      shaderProgram.setVec4f("vertexColor", color);
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