#pragma once

#include <libs/core/Layer.hpp>
#include <libs/events/WindowEvent.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <vector>

struct Size {

  int width, height;
};

class AugmentedLogoLayer : public libs::core::Layer {
public:
  AugmentedLogoLayer(const std::string &name = "AugmentedLogoLayer");

  virtual ~AugmentedLogoLayer() = default;

  virtual void onUpdate() override;
  virtual void onEvent(libs::events::Event &event) override;

private:
  bool onWindowResized(libs::events::WindowResizeEvent &event);

  Size m_windowSize;
  unsigned int m_vaoBackground, m_vboBackground, m_ebo;
  std::vector<float> m_vertices{
      -1.0f, -1.0f, 0.0f, 0.f, 0.f, -1.0f, 1.0f,  0.0f, 0.f, 1.f,
      1.0f,  1.0f,  0.0f, 1.f, 1.f, 1.0f,  -1.0f, 0.0f, 1.f, 0.f,
  };
  std::vector<int> m_indices{0, 1, 2, 0, 2, 3};
  libs::renderer::ShaderManager m_shaderManager{};
};