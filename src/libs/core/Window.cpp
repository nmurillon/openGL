#include <libs/core/Window.hpp>

#include <libs/core/Viewport.hpp>
#include <libs/events/EventDispatcher.hpp>
#include <libs/events/KeyEvent.hpp>
#include <libs/events/MouseEvent.hpp>
#include <libs/events/WindowEvent.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdexcept>
namespace libs::core {
Window::Window(float width, float height, const std::string &title)
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

  glfwSetWindowUserPointer(m_window, &m_eventCallback);

  glfwSetFramebufferSizeCallback(
      m_window, [](GLFWwindow *window, int width, int height) {
        libs::events::WindowResizeEvent event{static_cast<float>(width),
                                              static_cast<float>(height)};
        auto callback =
            *static_cast<EventCallbackFn *>(glfwGetWindowUserPointer(window));
        callback(event);
      });

  glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int mods) {
    auto callback =
        *static_cast<EventCallbackFn *>(glfwGetWindowUserPointer(window));
    switch (action) {
    case GLFW_PRESS: {
      libs::events::KeyPressedEvent event{key, false};
      callback(event);
      break;
    }
    case GLFW_RELEASE: {
      libs::events::KeyReleasedEvent event{key};
      callback(event);
      break;
    }
    case GLFW_REPEAT: {
      libs::events::KeyPressedEvent event{key, true};
      callback(event);
      break;
    }
    default:
      break;
    }
  });

  glfwSetCursorPosCallback(
      m_window, [](GLFWwindow *window, double xpos, double ypos) {
        auto callback =
            *static_cast<EventCallbackFn *>(glfwGetWindowUserPointer(window));
        libs::events::MouseMovedEvent event{xpos, ypos};
        callback(event);
      });

  glfwSetScrollCallback(
      m_window, [](GLFWwindow *window, double xoffset, double yoffset) {
        auto callback =
            *static_cast<EventCallbackFn *>(glfwGetWindowUserPointer(window));
        libs::events::MouseScrolledEvent event{xoffset, yoffset};
        callback(event);
      });

  glfwSetDropCallback(
      m_window, [](GLFWwindow *window, int count, const char **paths) {
        auto callback =
            *static_cast<EventCallbackFn *>(glfwGetWindowUserPointer(window));
        libs::events::PathDropEvent event{count, paths};
        callback(event);
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

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init();
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

void Window::setEventCallback(const EventCallbackFn &eventCallback) {
  m_eventCallback = eventCallback;
}

void Window::onUpdate() {
  glfwPollEvents();

  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window, true);
  }

  static double lastFrame{glfwGetTime()};
  double currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  glfwSwapBuffers(m_window);

  // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(Window::onWindowResized));
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

const int &Window::getWidth() const { return m_width; }

const int &Window::getHeight() const { return m_height; }

const double &Window::getDeltaTime() const { return m_deltaTime; }

double Window::getFps() const {
  double fps{0.};
  if (0. != m_deltaTime) {
    fps = 1.0 / m_deltaTime;
  }
  return fps;
}

// This fails on reldeb but works on debug
bool Window::onWindowResized(libs::events::WindowResizeEvent &event) {
  m_width = event.getWidth();
  m_height = event.getHeight();

  glViewport(0, 0, m_width, m_height);

  libs::core::Viewport::setWindowSize(m_width, m_height);

  return false;
}
} // namespace libs::core