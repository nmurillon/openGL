#include <libs/core/LayerStack.hpp>

#include <algorithm>
#include <iostream>
#include <ranges>

namespace libs::core {

void LayerStack::addLayer(const std::shared_ptr<Layer> &layer) {
  m_layers.emplace_back(layer);
}

void LayerStack::addOverlayLayer(const std::shared_ptr<Layer> &layer) {
  m_uiLayers.emplace_back(layer);
}

void LayerStack::removeLayer(const std::shared_ptr<Layer> &layer) {
  auto it = std::find(m_layers.begin(), m_layers.end(), layer);

  if (it != m_layers.end()) {
    m_layers.erase(it);
  }
}

void LayerStack::removeOverlayLayer(const std::shared_ptr<Layer> &layer) {
  auto it = std::find(m_uiLayers.begin(), m_uiLayers.end(), layer);

  if (it != m_uiLayers.end()) {
    m_uiLayers.erase(it);
  }
}

void LayerStack::onUpdate() {
  for (const auto &l : m_layers) {
    l->onUpdate();
  }

  for (const auto &l : m_uiLayers) {
    l->onUpdate();
  }
}

void LayerStack::onEvent(events::Event &event) {
  for (const auto &layer : std::ranges::reverse_view(m_uiLayers)) {
    layer->onEvent(event);
    if (event.isHandled()) {
      break;
    }
  }

  for (const auto &layer : std::ranges::reverse_view(m_layers)) {
    layer->onEvent(event);
    if (event.isHandled()) {
      break;
    }
  }
}

void LayerStack::printStack() const {
  for (const auto &l : m_layers) {
    std::cout << "Layer: " << l->getName() << std::endl;
  }

  for (const auto &l : m_uiLayers) {
    std::cout << "UI Layer: " << l->getName() << std::endl;
  }
}

} // namespace libs::core