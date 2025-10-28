#include <libs/core/Application.hpp>

namespace libs::core {
Application::Application(const std::string &name)
    : m_mainWindow(std::make_unique<Window>(800, 600, name)) {}

Application::~Application() {}

void Application::run() {
  while (!m_mainWindow->shouldClose()) {
    m_mainWindow->onUpdate();
  }
}

Window &Application::getWindow() { return *m_mainWindow; }

const Window &Application::getWindow() const { return *m_mainWindow; }
} // namespace libs::core