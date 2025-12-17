#include <libs/renderer/Texture.hpp>

#include <stb_image/stb_image.h>

#include <iostream>

namespace libs::renderer {

std::string textureTypeToString(TextureType type) {
  switch (type) {
  case TextureType::DIFFUSE:
    return "texture_diffuse";
  case TextureType::SPECULAR:
    return "texture_specular";
  default:
    return "unknown";
  }
}

std::map<std::filesystem::path, Texture::Data> Texture::s_loadedTextures;

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
  int width, height, nChannels;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(path.string().c_str(), &width, &height, &nChannels, 0);

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    std::cout << "WARNING::TEXTURE::FAIL_TO_OPEN" << std::endl;
  }

  stbi_image_free(data);
}

GLuint Texture::id() const { return m_data.id; }

TextureType Texture::type() const { return m_data.type; }
} // namespace libs::renderer