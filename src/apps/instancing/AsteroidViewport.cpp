#pragma once

#include "AsteroidViewport.hpp"

#include <libs/renderer/PerspectiveCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include <vector>

AsteroidViewport::AsteroidViewport(const std::string &name, float width,
                                   float height,
                                   std::filesystem::path assetsDir)
    : libs::core::Viewport(name, width, height), m_assetsDir(assetsDir),
      m_camera(std::make_shared<libs::renderer::PerspectiveCamera>(
          glm::vec3(0.f, 0.0f, 55.0f))),
      m_cameraController(m_camera) {

  m_planet = std::make_shared<libs::renderer::Model>(
      (m_assetsDir / "textures" / "planet" / "planet.obj").string());

  m_asteroid = std::make_shared<libs::renderer::Model>(
      (m_assetsDir / "textures" / "rock" / "rock.obj").string());

  m_shaderManager.addShader("planet",
                            (m_assetsDir / "shaders" / "planet.vert").string(),
                            (m_assetsDir / "shaders" / "planet.frag").string());

  m_shaderManager.addShader(
      "asteroid", (m_assetsDir / "shaders" / "asteroid.vert").string(),
      (m_assetsDir / "shaders" / "asteroid.frag").string());

  srand(glfwGetTime());
  std::vector<glm::mat4> modelMatrixes;
  modelMatrixes.reserve(m_maxAsteroidCount);

  const float radius = 50.f;
  const float offset = 2.5f;

  for (unsigned int i = 0; i < m_maxAsteroidCount; ++i) {
    glm::mat4 model = glm::mat4(1.0f);

    // 1. translation: displace along circle with 'radius' in range [-offset,
    // offset]
    const float angle =
        static_cast<float>(i) / static_cast<float>(m_maxAsteroidCount) * 360.f;
    float displacement =
        (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;

    float x = sin(angle) * radius + displacement;
    displacement =
        (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
    float y = displacement *
              0.4f; // keep height of field smaller compared to width of x and z
    displacement =
        (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. scale: scale between 0.05 and 0.25f
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    // 3. rotation: add random rotation around a (semi)randomly picked rotation
    // axis vector
    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. now add to list of matrices
    modelMatrixes.push_back(model);
  }

  for (auto &mesh : m_asteroid->getMeshes()) {
    mesh.getVertexArray().addVertexBuffer(libs::renderer::VertexBuffer{
        libs::renderer::BufferLayout{sizeof(glm::mat4),
                                     {{.m_size = sizeof(glm::vec4),
                                       .m_count = 4,
                                       .m_offset = 0,
                                       .m_attribDivisor = 1},
                                      {.m_size = sizeof(glm::vec4),
                                       .m_count = 4,
                                       .m_offset = sizeof(glm::vec4),
                                       .m_attribDivisor = 1},
                                      {.m_size = sizeof(glm::vec4),
                                       .m_count = 4,
                                       .m_offset = 2 * sizeof(glm::vec4),
                                       .m_attribDivisor = 1},
                                      {.m_size = sizeof(glm::vec4),
                                       .m_count = 4,
                                       .m_offset = 3 * sizeof(glm::vec4),
                                       .m_attribDivisor = 1}}},
        modelMatrixes.data(), m_maxAsteroidCount * sizeof(glm::mat4)});
  }
}

void AsteroidViewport::onEvent(libs::events::Event &event) {
  ImGuiIO &io = ImGui::GetIO();
  const auto mousePos = io.MousePos;
  if (isActive() && isInViewport(mousePos.x, mousePos.y)) {
    m_cameraController.onEvent(event);
  }
};

void AsteroidViewport::onImguiUpdate() {
  ImGui::Begin("Asteroid settings");
  ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
  ImGui::SliderInt("Number of asteroids", &m_asteroidCount, 0,
                   m_maxAsteroidCount);
  ImGui::End();
}

void AsteroidViewport::initState() { m_openglStateCache->setDepthTest(true); }
void AsteroidViewport::resetState() { m_openglStateCache->setDepthTest(false); }

void AsteroidViewport::drawScene() {
  drawPlanet();
  drawAsteroids();
}

void AsteroidViewport::drawPlanet() {
  m_cameraController.update();

  auto shader = m_shaderManager.getShader("planet");
  shader->use();

  glm::mat4 model = glm::mat4(1.f);
  model = glm::scale(model, glm::vec3(4.0));

  shader->setMat4f("model", model);
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());
  m_planet->draw(*shader);
}

void AsteroidViewport::drawAsteroids() {
  auto shader = m_shaderManager.getShader("asteroid");
  shader->use();

  //   shader->setMat4f("model", glm::mat4(1.f));
  shader->setMat4f("projection", m_camera->getProjection());
  shader->setMat4f("view", m_camera->getViewMatrix());

  m_asteroid->drawInstanced(*shader, m_asteroidCount);
  auto a = glGetError();
  int i = 0;
}