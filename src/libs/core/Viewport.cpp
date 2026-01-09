#include <libs/core/Viewport.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace libs::core {
float Viewport::s_windowWidth = 800.f;
float Viewport::s_windowHeight = 600.f;

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
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.f, 1.f));
  ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 150.0f),
                                      ImVec2(FLT_MAX, FLT_MAX));

  const bool isCurrentTab = ImGui::Begin(m_name.c_str());

  // This does not take into account the height of the window name
  // see https://github.com/ocornut/imgui/issues/2486
  //   const auto windowPos = ImGui::GetWindowPos(); // Top left of the window
  //   const auto contentAvailable = ImGui::GetContentRegionAvail();
  //   const auto mainViewPort = ImGui::GetMainViewport()->Size;

  //   m_xBottomLeft = windowPos.x;
  //   m_yBottomLeft = mainViewPort.y - contentAvailable.y - windowPos.y;
  //   m_width = contentAvailable.x;
  //   m_height = contentAvailable.y;

  ImVec2 vMin = ImGui::GetWindowContentRegionMin();
  ImVec2 vMax = ImGui::GetWindowContentRegionMax();
  const auto windowPos = ImGui::GetWindowPos();
  const auto contentAvailable = ImGui::GetContentRegionAvail();
  const auto mainViewPort = ImGui::GetMainViewport()->Size;

  vMin.x += windowPos.x;
  vMin.y += windowPos.y;
  vMax.x += windowPos.x;
  vMax.y += windowPos.y;

  m_xBottomLeft = vMin.x;
  m_yBottomLeft = mainViewPort.y - vMax.y;
  m_width = vMax.x - vMin.x;
  m_height = vMax.y - vMin.y;

  // Debug info
  //   ImGui::GetForegroundDrawList()->AddRect(vMin, vMax,
  //                                           IM_COL32(255, 255, 0, 255));

  m_isActive = isCurrentTab && !ImGui::IsWindowCollapsed();

  // ImVec2 p = ImGui::GetWindowPos();
  // ImGui::GetForegroundDrawList()->AddCircle(p, 10.0f, IM_COL32(255, 0, 0,
  // 255));

  ImGui::End();
  ImGui::PopStyleVar();
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

bool Viewport::isInViewport(float x, float y) {
  return m_xBottomLeft <= x && x <= m_xBottomLeft + m_width &&
         m_yBottomLeft <= y && y <= m_yBottomLeft + m_height;
}

void Viewport::setWindowSize(float width, float height) {
  s_windowWidth = width;
  s_windowHeight = height;
}

} // namespace libs::core