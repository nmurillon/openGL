#include <libs/core/Viewport.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace libs::core {
Viewport::Viewport(const std::string &name, float width, float height)
    : m_name(name), m_width(width), m_height(height) {}

void Viewport::prepareScene() {
  glViewport(m_xBottomLeft, m_yBottomLeft, m_width, m_height);
  // TODO: use custom background color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewport::display() {

  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 150.0f),
                                      ImVec2(FLT_MAX, FLT_MAX));

  const bool isCurrentTab = ImGui::Begin(m_name.c_str());
  const auto windowPos = ImGui::GetWindowPos(); // Top left of the window
  const auto contentAvailable = ImGui::GetContentRegionAvail();

  // TODO: rework this
  m_xBottomLeft = windowPos.x;
  m_yBottomLeft = -(windowPos.y - contentAvailable.y);
  m_width = contentAvailable.x;
  m_height = contentAvailable.y;

  m_isActive = isCurrentTab && !ImGui::IsWindowCollapsed();

  ImGui::End();
  //   ImGui::PopStyleVar();
  ImGui::PopStyleColor();
}

void Viewport::onUpdate() {
  if (!isActive()) {
    return;
  }

  prepareScene();
  drawScene();
}

bool Viewport::isActive() { return m_isActive; }
} // namespace libs::core