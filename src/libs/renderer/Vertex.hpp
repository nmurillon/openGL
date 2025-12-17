#pragma once

#include <logl/renderer/export.h>

#include <glm/glm.hpp>

namespace libs::renderer {
struct LOGL_RENDERER_EXPORT Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};
} // namespace libs::renderer