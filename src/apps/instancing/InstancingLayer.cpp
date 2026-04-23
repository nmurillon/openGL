#include "InstancingLayer.hpp"

#include "QuadViewport.hpp"

#include <libs/io/ProgramPath.hpp>

InstancingLayer::InstancingLayer(const std::string &name) : Layer(name) {
  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          INSTANCING_RESOURCES_FOLDER / "assets");

  m_viewports.emplace_back(std::make_shared<QuadViewport>(
      "Quad instancing", 800.f, 600.f, assetsDir));
}

void InstancingLayer::onUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->onUpdate();
  }
}

void InstancingLayer::onImguiUpdate() {
  for (const auto &viewport : m_viewports) {
    viewport->display();
    viewport->onImguiUpdate();
  }
}

void InstancingLayer::onEvent(libs::events::Event &event) {
  for (const auto &viewport : m_viewports) {
    viewport->onEvent(event);
  }
}