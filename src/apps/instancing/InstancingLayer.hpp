#pragma once

#include <libs/core/Layer.hpp>
#include <libs/core/Viewport.hpp>

#include <memory>
#include <string>
#include <vector>

class InstancingLayer : public libs::core::Layer {

public:
  InstancingLayer(const std::string &name = "InstancingLayer");

  virtual ~InstancingLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  std::vector<std::shared_ptr<libs::core::Viewport>> m_viewports{};
};