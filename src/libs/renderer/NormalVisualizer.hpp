#pragma once

#include <libs/renderer/Model.hpp>
#include <libs/renderer/ShaderManager.hpp>

#include <glm/glm.hpp>

namespace libs::renderer {
class NormalVisualizer {
public:
  NormalVisualizer(int bindingPoint = 0);
  ~NormalVisualizer() = default;

  void draw(const Model &);
  void drawSettingsWindow();

  const float &getMagnitude();
  void setMagnitude(float);

  const glm::vec4 &getColor();
  void setColor(const glm::vec4 &);

protected:
  ShaderManager m_shaderManager;
  float m_magnitude{0.4};
  glm::vec4 m_color{1.0, 1.0, 0.0, 1.0};
};
} // namespace libs::renderer