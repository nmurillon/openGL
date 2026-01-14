#include "AdvancedOpenGlLayer.hpp"

AdvancedOpenGlLayer::AdvancedOpenGlLayer(const std::string &name)
    : Layer(name), m_depthTestViewport("Depth Test") {}

void AdvancedOpenGlLayer::onUpdate() { m_depthTestViewport.onUpdate(); }

void AdvancedOpenGlLayer::onImguiUpdate() {
  m_depthTestViewport.display();
  m_depthTestViewport.onImguiUpdate();
}

void AdvancedOpenGlLayer::onEvent(libs::events::Event &event) {
  m_depthTestViewport.onEvent(event);
}