#include "GeometryShaderLayer.hpp"

#include "HousesViewport.hpp"

#include <libs/io/ProgramPath.hpp>

GeometryShaderLayer::GeometryShaderLayer(const std::string &name)
    : Layer(name) {
  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          GEOMETRYSHADER_RESOURCES_FOLDER / "assets");

  m_viewports.emplace_back(
      std::make_shared<HousesViewport>("Houses", 800.f, 600.f, assetsDir));
}

void GeometryShaderLayer::onUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->onUpdate();
  }
}

void GeometryShaderLayer::onImguiUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->display();
    viewport->onImguiUpdate();
  }
}

void GeometryShaderLayer::onEvent(libs::events::Event &event) {
  for (const auto &viewport : m_viewports) {
    viewport->onEvent(event);
  }
}