#include <libs/renderer/Texture.hpp>

#include <libs/logger/Logger.hpp>

#include <stb_image/stb_image.h>

namespace libs::renderer {

std::string textureTypeToString(TextureType type) {
  switch (type) {
  case TextureType::DIFFUSE:
    return "texture_diffuse";
  case TextureType::SPECULAR:
    return "texture_specular";
  case TextureType::BUFFER:
    return "texture_buffer";
  default:
    return "unknown";
  }
}

std::map<std::filesystem::path, Texture::Data> Texture::s_loadedTextures;

Texture::Texture(int width, int height) {
  m_data.type = TextureType::BUFFER;
  glGenTextures(1, &m_data.id);
  glBindTexture(GL_TEXTURE_2D, m_data.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);

  // Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(Data data) : m_data(data) {}

Texture::Texture(TextureType type, const std::string &path)
    : Texture(type, std::filesystem::path(path)) {}

Texture::Texture(TextureType type, const std::filesystem::path &path) {
  const auto it = s_loadedTextures.find(path);

  if (it == s_loadedTextures.end()) {
    load(path);
    m_data.type = type;
    s_loadedTextures[path] = m_data;
  } else {
    m_data = it->second;
  }
}

void Texture::load(const std::filesystem::path &path) {
  int nChannels;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.string().c_str(), &m_data.width,
                                  &m_data.height, &nChannels, 0);

  glGenTextures(1, &m_data.id);
  glBindTexture(GL_TEXTURE_2D, m_data.id);

  if (data) {

    GLenum format;
    if (nChannels == 1) {
      format = GL_RED;
    } else if (nChannels == 3) {
      format = GL_RGB;
    } else if (nChannels == 4) {
      format = GL_RGBA;
    }

    m_data.format = format;

    glTexImage2D(GL_TEXTURE_2D, 0, m_data.format, m_data.width, m_data.height,
                 0, m_data.format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    Logger::logWarning("WARNING::TEXTURE::FAIL_TO_OPEN {}", path.string());
  }

  stbi_image_free(data);
}

GLuint Texture::id() const { return m_data.id; }

TextureType Texture::type() const { return m_data.type; }

void Texture::setTextureWrap(GLint wrapS, GLint wrapT) {
  glBindTexture(GL_TEXTURE_2D, m_data.id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::setSize(int width, int height) {
  if (m_data.type != TextureType::BUFFER) {
    Logger::logError(
        "ERROR::TEXTURE::SET_SIZE can only be called on BUFFER textures");
    return;
  }

  m_data.width = width;
  m_data.height = height;
  glBindTexture(GL_TEXTURE_2D, m_data.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
}

void Texture::bind(GLenum target) const { glBindTexture(target, m_data.id); }

void Texture::unbind(GLenum target) { glBindTexture(target, 0); }

} // namespace libs::renderer