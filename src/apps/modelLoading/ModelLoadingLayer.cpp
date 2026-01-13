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