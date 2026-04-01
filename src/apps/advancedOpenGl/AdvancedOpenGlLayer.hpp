#pragma once

#include <libs/core/Layer.hpp>

#include "BlendingViewport.hpp"
#include "DepthTestViewport.hpp"

#include <memory>
#include <string>
#include <vector>

class AdvancedOpenGlLayer : public libs::core::Layer {

public:
  AdvancedOpenGlLayer(const std::string &name = "AdvancedOpenGlLayer");

  virtual ~AdvancedOpenGlLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  std::vector<std::shared_ptr<libs::core::Viewport>> m_viewports{
      // std::make_shared<DepthTestViewport>("Depth Test"),
      // std::make_shared<BlendingViewport>("Blending")};
  };
};