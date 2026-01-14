#pragma once

#include <libs/core/Layer.hpp>

#include "DepthTestViewport.hpp"

#include <string>

class AdvancedOpenGlLayer : public libs::core::Layer {

public:
  AdvancedOpenGlLayer(const std::string &name = "AdvancedOpenGlLayer");

  virtual ~AdvancedOpenGlLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  DepthTestViewport m_depthTestViewport;
};