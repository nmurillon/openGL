#pragma once

#include <libs/core/Layer.hpp>
#include <libs/core/Viewport.hpp>

#include <memory>
#include <string>
#include <vector>

class UniformBufferLayer : public libs::core::Layer {

public:
  UniformBufferLayer(const std::string &name = "UniformBufferLayer");

  virtual ~UniformBufferLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  std::vector<std::shared_ptr<libs::core::Viewport>> m_viewports{};
};