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

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
} // namespace libs::renderer