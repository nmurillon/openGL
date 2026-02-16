#pragma once

#include <logl/renderer/export.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <filesystem>
#include <map>
#include <string>

namespace libs::renderer {

enum class TextureType { DIFFUSE, SPECULAR };

std::string textureTypeToString(TextureType type);

class LOGL_RENDERER_EXPORT Texture {
public:
  Texture(TextureType type, const std::string &path);
  Texture(TextureType type, const std::filesystem::path &path);

  GLuint id() const;
  TextureType type() const;
  void setTextureWrap(GLint wrapS, GLint wrapT);

private:
  struct Data {
    GLuint id;
    TextureType type;
  };

  Data m_data;

private:
  Texture(Data data);

  void load(const std::filesystem::path &path);

  static std::map<std::filesystem::path, Data> s_loadedTextures;
};
} // namespace libs::renderer