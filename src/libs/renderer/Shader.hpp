#pragma once

#include <logl/renderer/export.h>

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Shader {
public:
  Shader(const std::string &vertexSrcFile, const std::string &fragmentSrcFile);
  void use() const;

  // Utility functions to help set uniform values;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3f(const std::string &name, const glm::vec3 &value) const;
  void setVec3f(const std::string &name, const std::vector<float> &value) const;
  void setVec3f(const std::string &name, float x, float y, float z) const;
  void setVec4f(const std::string &name, const glm::vec4 &value) const;
  void setVec4f(const std::string &name, const std::vector<float> &value) const;
  void setMat4f(const std::string &name, const glm::mat4 &mat) const;

private:
  unsigned int m_shaderId;

  /**
   * @brief Create and compile a shader given a source file
   * @param [in] srcFile The path to the file containing the shader's code
   * @param [in] type The type of shader to create
   * @return The id of the shader
   */
  unsigned int readShaderFile(const std::string &srcFile, unsigned int type);

  GLint getLocation(const std::string &name) const;
};
} // namespace libs::renderer