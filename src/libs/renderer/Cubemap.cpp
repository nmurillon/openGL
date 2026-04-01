#include <libs/renderer/Cubemap.hpp>

#include <libs/logger/Logger.hpp>

#include <stb_image/stb_image.h>

namespace libs::renderer {
Cubemap::Cubemap() {}

Cubemap::Cubemap(const std::vector<std::string> &faces) {
  if (faces.size() != 6) {
    Logger::logError("Cubemap must have 6 textures, {} were provided",
                     faces.size());
  }

  m_vao.setVertexBuffer(
      VertexBuffer{BufferLayout{BufferLayoutElement{sizeof(float), 3}},
                   m_vertices.data(), m_vertices.size() * sizeof(float)});

  glGenTextures(1, &m_id);
  bind();

  for (int i = 0; i < faces.size(); ++i) {
    load(faces.at(i), i);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::Cubemap(Cubemap &&other)
    : m_id(other.m_id), m_vao(std::move(other.m_vao)) {
  other.m_id = 0;
}

Cubemap::~Cubemap() { glDeleteTextures(1, &m_id); }

Cubemap &Cubemap::operator=(Cubemap &&other) {
  if (this == &other) {
    return *this;
  }

  if (m_id) {
    glDeleteTextures(1, &m_id);
  }

  m_id = other.m_id;
  m_vao = std::move(other.m_vao);

  other.m_id = 0;

  return *this;
}

void Cubemap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_id); }

void Cubemap::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

void Cubemap::draw() const {
  m_vao.bind();
  bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  unbind();
  m_vao.unbind();
}

void Cubemap::load(const std::string &face, int target) {
  int nChannels, width, height;

  unsigned char *data = stbi_load(face.c_str(), &width, &height, &nChannels, 0);

  if (data) {

    GLenum format = GL_RGB;
    if (nChannels == 1) {
      format = GL_RED;
    } else if (nChannels == 3) {
      format = GL_RGB;
    } else if (nChannels == 4) {
      format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + target, 0, format, width,
                 height, 0, format, GL_UNSIGNED_BYTE, data);
  } else {
    Logger::logWarning("WARNING::TEXTURE::FAIL_TO_OPEN {}", face);
  }

  stbi_image_free(data);
}
} // namespace libs::renderer