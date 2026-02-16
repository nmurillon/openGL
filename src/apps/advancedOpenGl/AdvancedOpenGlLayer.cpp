#include "AdvancedOpenGlLayer.hpp"

AdvancedOpenGlLayer::AdvancedOpenGlLayer(const std::string &name)
    : Layer(name) {}

void AdvancedOpenGlLayer::onUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->onUpdate();
  }
}

void AdvancedOpenGlLayer::onImguiUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->display();
    viewport->onImguiUpdate();
  }
}

void AdvancedOpenGlLayer::onEvent(libs::events::Event &event) {
  for (const auto &viewport : m_viewports) {
    viewport->onEvent(event);
  }
}