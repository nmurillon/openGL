#pragma once

#include <logl/core/export.h>

#include <libs/core/Layer.hpp>

#include <memory>
#include <vector>

namespace libs::core {

using Layers = std::vector<std::shared_ptr<Layer>>;
class LOGL_CORE_EXPORT LayerStack {
public:
  LayerStack() = default;
  ~LayerStack() = default;

  void addLayer(const std::shared_ptr<Layer> &);
  void addOverlayLayer(const std::shared_ptr<Layer> &);
  void removeLayer(const std::shared_ptr<Layer> &);
  void removeOverlayLayer(const std::shared_ptr<Layer> &);

  void onUpdate();
  void onEvent(events::Event &event);

  void printStack() const;

private:
  void imguiStart();
  void imguiEnd();

  Layers m_layers;
  Layers m_uiLayers;
};
} // namespace libs::core