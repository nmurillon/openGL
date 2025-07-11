#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const char *vertexShaderSrc =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSrc = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                "}\0";

const char *fragmentShaderYellowSrc =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 1.f, 0.f, 1.0f);\n"
    "}\0";

// Callback to resize the window if user resizes it
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Handle user inputs to close window if escaped is pressed
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // SETUP SHADERS
  const uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  const uint fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragementShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragementShader);

  const uint shaderProgram = glCreateProgram();

  // Beware to the order
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragementShader);

  glLinkProgram(shaderProgram);

  // Once linked, shaders can be deleted
  glDeleteShader(vertexShader);
  glDeleteShader(fragementShader);

  // SETUP VERTEX DATA
  // clang-format off
  std::vector<std::vector<double>> vertices = {{-0.5f, -0.5f, 0.f,
                                                   0.f, 0.5f, 0.f,
                                                   0.5f, -0.5f, 0.f},
                                               {-1.f, 1.f, 0.f,
                                                    0.f, 1.f, 0.f,
                                                    -0.5f, 0.f, 0.f,
                                                    // Second triangle
                                                    0.f, 1.f, 0.f,
                                                    1.f, 1.f, 0.f,
                                                    0.5f, 0.f, 0.f}};

  // clang-format on

  // VAO & VBO
  std::vector<uint> VAO(2), VBO(2);
  glGenVertexArrays(2, VAO.data());
  glGenBuffers(2, VBO.data());

  for (int i = 0; i < 2; ++i) {
    glBindVertexArray(VAO.at(i));

    glBindBuffer(GL_ARRAY_BUFFER, VBO.at(i));
    glBufferData(GL_ARRAY_BUFFER, vertices.at(i).size() * sizeof(double),
                 vertices.at(i).data(), GL_STATIC_DRAW);

    // Link Vertex attributes
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double),
                          (void *)0);
    glEnableVertexAttribArray(0);
  }

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, 800, 600);

  // Wait for user input to keep the window opened
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    for (int i = 0; i < 2; ++i) {
      glBindVertexArray(VAO.at(i));
      glDrawArrays(GL_TRIANGLES, 0, vertices.at(i).size() / 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean data before exiting
  glfwTerminate();

  return 0;
}