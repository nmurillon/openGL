#include "DepthTestViewport.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>
#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

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
  m_camera->setViewportSize(m_width, m_height);
  m_cameraController.update();

  auto shader = m_shaderManager.getShader("model");
  shader->use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  model = glm::scale(model, glm::vec3(1.0f));

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  glBindVertexArray(m_cubeVAO);
  shader->setInt("tex", 0);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  shader->setMat4f("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // floor
  glBindVertexArray(m_planeVAO);
  shader->setInt("tex", 1);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}

void DepthTestViewport::onImguiUpdate() {}

void DepthTestViewport::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher(event);

  ImGuiIO &io = ImGui::GetIO();
  const auto mousePos = io.MousePos;
  if (isInViewport(mousePos.x, s_windowHeight - mousePos.y)) {
    m_cameraController.onEvent(event);
  }
}