#include <libs/core/LayerStack.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <algorithm>
#include <iostream>
#include <ranges>

namespace libs::core {

void LayerStack::addLayer(const std::shared_ptr<Layer> &layer) {
  m_layers.emplace_back(layer);
}

void LayerStack::addOverlayLayer(const std::shared_ptr<Layer> &layer) {
  m_uiLayers.emplace_back(layer);
}

void LayerStack::removeLayer(const std::shared_ptr<Layer> &layer) {
  auto it = std::find(m_layers.begin(), m_layers.end(), layer);

  if (it != m_layers.end()) {
    m_layers.erase(it);
  }
}

void LayerStack::removeOverlayLayer(const std::shared_ptr<Layer> &layer) {
  auto it = std::find(m_uiLayers.begin(), m_uiLayers.end(), layer);

  if (it != m_uiLayers.end()) {
    m_uiLayers.erase(it);
  }
}

void LayerStack::onUpdate() {
  for (const auto &l : m_layers) {
    l->onUpdate();
  }

  for (const auto &l : m_uiLayers) {
    l->onUpdate();
  }

  imguiStart();
  for (const auto &l : m_layers) {
    l->onImguiUpdate();
  }

  for (const auto &l : m_uiLayers) {
    l->onImguiUpdate();
  }
  imguiEnd();
}

void LayerStack::onEvent(events::Event &event) {
  for (const auto &layer : std::ranges::reverse_view(m_uiLayers)) {
    layer->onEvent(event);
    if (event.isHandled()) {
      break;
    }
  }

  for (const auto &layer : std::ranges::reverse_view(m_layers)) {
    layer->onEvent(event);
    if (event.isHandled()) {
      break;
    }
  }
}

void LayerStack::printStack() const {
  for (const auto &l : m_layers) {
    std::cout << "Layer: " << l->getName() << std::endl;
  }

  for (const auto &l : m_uiLayers) {
    std::cout << "UI Layer: " << l->getName() << std::endl;
  }
}

void LayerStack::imguiStart() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // ImGui provides DockSpaceOverViewport, but even with the
  // ImGuiDockNodeFlags_PassthruCentralNode flag set, the background still gets
  // drawn
  ImGuiIO &io = ImGui::GetIO();
  ImGuiDockNodeFlags dockspace_flags =
      ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", nullptr, window_flags);

  // Submit the DockSpace
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  ImGui::End();

  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor();
}

void LayerStack::imguiEnd() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace libs::core