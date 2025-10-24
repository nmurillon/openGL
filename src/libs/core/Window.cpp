#include <libs/core/Window.hpp>

#include <libs/core/Camera.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdexcept>

namespace libs::core {
Window::Window(int width, int height, std::string title)
    : m_width(width), m_height(height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);

  if (m_window == NULL) {
    glfwTerminate();

    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_window);

  glfwSetFramebufferSizeCallback(m_window,
                                 [](GLFWwindow *window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

void Window::open(std::function<void(void)> frameHandler) {
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init();
  float currentFrame = glfwGetTime();
  float lastFrame = currentFrame;

  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(m_window, true);
    }

    currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (m_keyboardInputCallback) {
      m_keyboardInputCallback(m_window, m_deltaTime);
    }

    frameHandler();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("toto");
    ImGui::Text("FPS: %f", getFps());
    ImGui::SeparatorText("");
    ImGui::Text("FPS imgui: %f", ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
  }
}

void Window::setUserData(void *data) {
  glfwSetWindowUserPointer(m_window, data);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
  glfwSetScrollCallback(m_window, callback);
}

void Window::setCursorPosCallback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(m_window, callback);
}

void Window::setKeyboardInputCallback(
    std::function<void(GLFWwindow *, double)> callback) {
  m_keyboardInputCallback = callback;
}

const int &Window::getWidth() const { return m_width; }

const int &Window::getHeight() const { return m_height; }

const double &Window::getDeltaTime() const { return m_deltaTime; }

const double Window::getFps() const {
  double fps{0.};
  if (m_deltaTime) {
    fps = 1.0 / m_deltaTime;
  }
  return fps;
}
} // namespace libs::core