#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

#include <filesystem>
#include <map>
#include <string>

namespace libs::renderer {

enum class TextureType { DIFFUSE, SPECULAR, BUFFER };

std::string textureTypeToString(TextureType type);

class LOGL_RENDERER_EXPORT Texture {
public:
  // Constructor used for framebuffer textures --> Will probably need a rework
  // in the future (TODO: textures for stencil and depth)
  Texture(int width, int height);
  Texture(TextureType type, const std::string &path);
  Texture(TextureType type, const std::filesystem::path &path);

  GLuint id() const;
  TextureType type() const;
  void setTextureWrap(GLint wrapS, GLint wrapT);
  void setSize(int width, int height);
  void bind(GLenum target = GL_TEXTURE_2D) const;

  static void unbind(GLenum target = GL_TEXTURE_2D);

private:
  struct Data {
    GLuint id;
    TextureType type;
    GLenum format;

    int width;
    int height;
  };

  Data m_data;

private:
  Texture(Data data);

  void load(const std::filesystem::path &path);

  static std::map<std::filesystem::path, Data> s_loadedTextures;
};
} // namespace libs::renderer