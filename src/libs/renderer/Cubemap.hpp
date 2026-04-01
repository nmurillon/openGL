#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>
#include <libs/renderer/Texture.hpp>
#include <libs/renderer/VertexArray.hpp>

#include <array>

namespace libs::renderer {

class LOGL_RENDERER_EXPORT Cubemap {
public:
  Cubemap();
  // WARNING: for this to work, faces must be in that order: right, left, top,
  // bottom, front, back
  Cubemap(const std::vector<std::string> &faces);
  Cubemap(Cubemap &&);
  Cubemap(const Cubemap &) = delete;

  Cubemap &operator=(const Cubemap &) = delete;
  Cubemap &operator=(Cubemap &&);

  ~Cubemap();

  void bind() const;
  static void unbind();
  void draw() const;

private:
  void load(const std::string &face, int target);

private:
  std::array<float, 108> m_vertices{
      // clang-format off
      -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
      // clang-format on
  };

private:
  GLuint m_id;
  VertexArray m_vao{};
};
} // namespace libs::renderer