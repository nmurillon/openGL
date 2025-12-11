#pragma once

#include <logl/renderer/export.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>

namespace libs::renderer {

enum class TextureType { DIFFUSE, SPECULAR };

std::string textureTypeToString(TextureType type);

struct LOGL_RENDERER_EXPORT Texture {
  Texture(TextureType type, const std::string &path);
  Texture(TextureType type, const std::filesystem::path &path);

  GLuint id;
  TextureType type;

private:
  void load(const std::filesystem::path &path);
};
} // namespace libs::renderer