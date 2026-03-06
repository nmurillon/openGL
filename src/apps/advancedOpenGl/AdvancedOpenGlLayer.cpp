#include "AdvancedOpenGlLayer.hpp"

#include "FaceCullingViewport.hpp"
#include "FrameBufferViewport.hpp"
#include <libs/io/ProgramPath.hpp>

AdvancedOpenGlLayer::AdvancedOpenGlLayer(const std::string &name)
    : Layer(name) {
  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          ADVANCEDOPENGL_RESOURCES_FOLDER / "assets");

  m_viewports.emplace_back(std::make_shared<FaceCullingViewport>(
      "Face Culling", 800.f, 600.f, assetsDir));
  m_viewports.emplace_back(std::make_shared<FrameBufferViewport>(
      "Frame Buffer", 800.f, 600.f, assetsDir));
}

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