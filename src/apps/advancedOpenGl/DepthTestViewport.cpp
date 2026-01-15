#include "DepthTestViewport.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

namespace {
int depthFuncFromString(const std::string &funcStr) {
  if (funcStr == "GL_NEVER")
    return GL_NEVER;
  if (funcStr == "GL_LESS")
    return GL_LESS;
  if (funcStr == "GL_EQUAL")
    return GL_EQUAL;
  if (funcStr == "GL_LEQUAL")
    return GL_LEQUAL;
  if (funcStr == "GL_GREATER")
    return GL_GREATER;
  if (funcStr == "GL_NOTEQUAL")
    return GL_NOTEQUAL;
  if (funcStr == "GL_GEQUAL")
    return GL_GEQUAL;
  if (funcStr == "GL_ALWAYS")
    return GL_ALWAYS;
  return GL_ALWAYS; // Default
}
} // namespace

DepthTestViewport::DepthTestViewport(const std::string &name, float width,
                                     float height)
    : libs::core::Viewport(name, width, height),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 3.0f))),
      m_cameraController(m_camera) {

  const auto assetsDir = (libs::io::ProgramPath::getInstance().getProgramDir() /
                          ADVANCEDOPENGL_RESOURCES_FOLDER / "assets");

  const auto shaderDir = (assetsDir / "shaders").string();
  const auto textureDir = assetsDir / "textures";

  m_shaderManager.addShader("model",
                            std::format("{}/depthTesting.vert", shaderDir),
                            std::format("{}/depthTesting.frag", shaderDir));

  m_shaderManager.addShader("modelOutline",
                            std::format("{}/depthTesting.vert", shaderDir),
                            std::format("{}/outline.frag", shaderDir));

  // clang-format off
  float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };
  // clang-format on

  glGenVertexArrays(1, &m_cubeVAO);
  glGenBuffers(1, &m_cubeVBO);
  glBindVertexArray(m_cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));

  glGenVertexArrays(1, &m_planeVAO);
  glGenBuffers(1, &m_planeVBO);
  glBindVertexArray(m_planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Setup textures
  m_marble = {libs::renderer::TextureType::DIFFUSE,
              std::format("{}/marble.jpg", textureDir.string())};

  m_metal = {libs::renderer::TextureType::DIFFUSE,
             std::format("{}/metal.png", textureDir.string())};

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_metal.id());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_marble.id());

  glEnable(GL_DEPTH_TEST);
}

void DepthTestViewport::drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDepthFunc(m_depthFunc);

  if (m_showOutline) {
    glEnable(GL_STENCIL_TEST);
  }

  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  m_camera->setViewportSize(m_width, m_height);
  m_cameraController.update();

  // floor - Don't update the stencil buffer
  glStencilMask(0x00);
  drawFloor();

  // cubes
  // All fragments pass the stencil test + Enable writing to the stencil buffer
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  drawCubes();

  if (m_showOutline) {
    // Note: if the objects are aligned, the one in front will not be fully
    // outlined
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    drawCubesOutline();

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
  }

  glBindVertexArray(0);
}

void DepthTestViewport::onImguiUpdate() {
  static std::string current_item = m_depthFuncs[0];

  ImGui::Begin("Depth Test Settings");

  ImGui::Checkbox("Show depth buffer", &m_showDepthBuffer);

  if (!m_showDepthBuffer &&
      ImGui::BeginCombo("Depth function", current_item.c_str())) {
    for (const auto &item : m_depthFuncs) {
      bool is_selected = (current_item == item);
      if (ImGui::Selectable(item.c_str(), is_selected)) {
        m_depthFunc = depthFuncFromString(item);
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::SeparatorText("Outline settings");

  ImGui::Checkbox("Show outline", &m_showOutline);

  ImGui::ColorPicker3("Outline color", glm::value_ptr(m_outlineColor));
  ImGui::SliderFloat("Outline scale", &m_outlineScale, 1.0f, 1.5f);

  ImGui::End();
}

void DepthTestViewport::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);

  ImGuiIO &io = ImGui::GetIO();
  const auto mousePos = io.MousePos;
  if (isActive() && isInViewport(mousePos.x, s_windowHeight - mousePos.y)) {
    m_cameraController.onEvent(event);
  }
}

void DepthTestViewport::drawFloor() {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glBindVertexArray(m_planeVAO);
  shader->setInt("tex", 1);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DepthTestViewport::drawCubes() {
  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  model = glm::scale(model, glm::vec3(1.0f));

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setBool("showDepthBuffer", m_showDepthBuffer);

  glBindVertexArray(m_cubeVAO);
  shader->setInt("tex", 0);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  shader->setMat4f("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DepthTestViewport::drawCubesOutline() {
  auto shader = m_shaderManager.getShader("modelOutline");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  model = glm::scale(model, glm::vec3(m_outlineScale));

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  shader->setVec3f("outlineColor", m_outlineColor);

  glBindVertexArray(m_cubeVAO);
  shader->setInt("tex", 0);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  model = glm::scale(model, glm::vec3(m_outlineScale));
  shader->setMat4f("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}