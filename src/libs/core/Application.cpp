#include <libs/core/Application.hpp>

namespace libs::core {
Application::Application(const std::string &name)
    : m_mainWindow(std::make_unique<Window>(1920, 1080, name)) {
  m_mainWindow->setEventCallback(
      std::bind(&Application::onEvent, this, std::placeholders::_1));
}

Application::~Application() {}

void Application::run() {
  while (!m_mainWindow->shouldClose()) {
    m_layerStack.onUpdate();
    m_mainWindow->onUpdate();
  }
}

void Application::onEvent(events::Event &event) {
  m_mainWindow->onEvent(event);
  m_layerStack.onEvent(event);
}

void Application::addLayer(const std::shared_ptr<Layer> &layer) {
  m_layerStack.addLayer(layer);
}

void Application::addOverlayLayer(const std::shared_ptr<Layer> &layer) {
  m_layerStack.addOverlayLayer(layer);
}

Window &Application::getWindow() { return *m_mainWindow; }

const Window &Application::getWindow() const { return *m_mainWindow; }
} // namespace libs::core