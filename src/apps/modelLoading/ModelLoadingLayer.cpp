#include "ModelLoadingLayer.hpp"

ModelLoadingLayer::ModelLoadingLayer(const std::string &name)
    : Layer(name), m_modelLoadingViewport("Model Loading") {}

void ModelLoadingLayer::onUpdate() { m_modelLoadingViewport.onUpdate(); }

void ModelLoadingLayer::onImguiUpdate() {
  m_modelLoadingViewport.display();
  m_modelLoadingViewport.onImguiUpdate();
}

void ModelLoadingLayer::onEvent(libs::events::Event &event) {
  m_modelLoadingViewport.onEvent(event);
}

// bool ModelLoadingLayer::onWindowResized(
//     libs::events::WindowResizeEvent &event) {
//   float width = static_cast<float>(event.getWidth());
//   float height = static_cast<float>(event.getHeight());

//   m_aspectRatio = width / height;

//   m_camera->setViewportSize(width, height);
//   return event.handle();
// }