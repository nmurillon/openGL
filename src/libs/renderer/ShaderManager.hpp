#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Shader.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT ShaderManager {
public:
  ShaderManager();
  ~ShaderManager() = default;

  std::shared_ptr<Shader> getShader(const std::string &name) const;
  void addShader(const std::string &name, std::shared_ptr<Shader> shader);
  void addShader(const std::string &name, const std::string &vertexSrcFile,
                 const std::string &fragmentSrcFile);

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

  // TODO: add search path for shaders
};
} // namespace libs::renderer