#pragma once

#include <libs/core/Layer.hpp>

#include "ModelLoadingViewport.hpp"

#include <string>

class ModelLoadingLayer : public libs::core::Layer {

public:
  ModelLoadingLayer(const std::string &name = "ModelLoadingLayer");

  virtual ~ModelLoadingLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;
  virtual void onImguiUpdate() override;

private:
  ModelLoadingViewport m_modelLoadingViewport;
};