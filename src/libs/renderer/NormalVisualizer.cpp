#include <libs/renderer/NormalVisualizer.hpp>

#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/Shader.hpp>

#include <filesystem>
#include <memory>

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace libs::renderer {
NormalVisualizer::NormalVisualizer(int bindingPoint) {
  const auto shaderDir = libs::io::ProgramPath::getInstance().getProgramDir() /
                         LOGL_RENDERER_RESOURCES_FOLDER / "shaders";

  const std::shared_ptr<Shader> shader = std::make_shared<Shader>(
      shaderDir / "normalVis.vert", shaderDir / "normalVis.frag",
      shaderDir / "normalVis.geom");

  shader->setBindingPoint("Matrices", bindingPoint);

  m_shaderManager.addShader("normal", shader);
}

void NormalVisualizer::draw(const Model &model) {
  const auto shader = m_shaderManager.getShader("normal");

  shader->use();
  shader->setFloat("magnitude", m_magnitude);
  shader->setVec4f("color", m_color);

  model.draw(*shader);
}

void NormalVisualizer::drawSettingsWindow() {
  ImGui::Begin("Normal visualization settings");
  ImGui::SliderFloat("Magnitude", &m_magnitude, 0.1f, 1.f);
  ImGui::ColorPicker4("Color", glm::value_ptr(m_color));
  ImGui::End();
}

const float &NormalVisualizer::getMagnitude() { return m_magnitude; }

void NormalVisualizer::setMagnitude(float magnitude) {
  m_magnitude = magnitude;
}

const glm::vec4 &NormalVisualizer::getColor() { return m_color; }

void NormalVisualizer::setColor(const glm::vec4 &color) { m_color = color; }
} // namespace libs::renderer