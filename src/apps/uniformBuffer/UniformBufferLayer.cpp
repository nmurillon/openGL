#include "UniformBufferLayer.hpp"

#include "UniformBufferViewport.hpp"

#include <libs/io/ProgramPath.hpp>

UniformBufferLayer::UniformBufferLayer(const std::string &name) : Layer(name) {
  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          UNIFORMBUFFER_RESOURCES_FOLDER / "assets");

  m_viewports.emplace_back(std::make_shared<UniformBufferViewport>(
      "Uniform buffer", 800.f, 600.f, assetsDir));
}

void UniformBufferLayer::onUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->onUpdate();
  }
}

void UniformBufferLayer::onImguiUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->display();
    viewport->onImguiUpdate();
  }
}

void UniformBufferLayer::onEvent(libs::events::Event &event) {
  for (const auto &viewport : m_viewports) {
    viewport->onEvent(event);
  }
}