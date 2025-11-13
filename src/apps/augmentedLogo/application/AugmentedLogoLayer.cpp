#include "AugmentedLogoLayer.hpp"

#include <libs/events/EventDispatcher.hpp>
#include <libs/io/ProgramPath.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <stb_image/stb_image.h>

unsigned int loadTexture(const std::string &textureFile,
                         GLenum textureUnit = GL_TEXTURE0,
                         GLint wrapping = GL_REPEAT) {
  int width, height, nChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(textureFile.c_str(), &width, &height, &nChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture);

  // Bind the texture to work on it
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "WARNING::TEXTURE::FAIL_TO_OPEN" << std::endl;
  }

  stbi_image_free(data);

  return texture;
}

AugmentedLogoLayer::AugmentedLogoLayer(const std::string &name) : Layer{name} {
  std::filesystem::path resourcesBaseDir =
      libs::io::ProgramPath::getInstance().getProgramDir() /
      AUGMENTED_LOGO_RESOURCES_FOLDER;

  m_shaderManager.addShader(
      "background",
      std::format("{}/shaders/backgroundVert.glsl", resourcesBaseDir.string()),
      std::format("{}/shaders/backgroundFrag.glsl", resourcesBaseDir.string()));

  auto texture = loadTexture(
      std::format("{}/textures/totk.jpg", resourcesBaseDir.string()),
      GL_TEXTURE0, GL_REPEAT);

  // Setup background quad
  glGenVertexArrays(1, &m_vaoBackground);
  glGenBuffers(1, &m_vboBackground);
  glGenBuffers(1, &m_ebo);

  glBindVertexArray(m_vaoBackground);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboBackground);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(sizeof(float) * m_vertices.size()),
               m_vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(m_indices.size() * sizeof(int)),
               m_indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AugmentedLogoLayer::onUpdate() {
  auto shader = m_shaderManager.getShader("background");
  shader->use();
  shader->setInt("iChannel0", 0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(m_vaoBackground);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                 GL_UNSIGNED_INT, 0);
}

void AugmentedLogoLayer::onEvent(libs::events::Event &event) {
  libs::events::EventDispatcher dispatcher{event};
  dispatcher.dispatch<libs::events::WindowResizeEvent>(
      LOGL_BIND_EVENT_FN(AugmentedLogoLayer::onWindowResized));
}

bool AugmentedLogoLayer::onWindowResized(
    libs::events::WindowResizeEvent &event) {
  m_windowSize.width = event.getWidth();
  m_windowSize.height = event.getHeight();
  return event.handle();
}