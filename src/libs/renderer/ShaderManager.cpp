#include <libs/renderer/ShaderManager.hpp>

#include <libs/io/ProgramPath.hpp>

#include <format>
#include <iostream>

namespace libs::renderer {
ShaderManager::ShaderManager() {
  const std::string shaderBasePath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       LOGL_RENDERER_RESOURCES_FOLDER / "shaders" / "basicShader")
          .string()};

  addShader("loglBasicShader", std::make_shared<libs::renderer::Shader>(
                                   std::format("{}.vert", shaderBasePath),
                                   std::format("{}.frag", shaderBasePath)));
}

void ShaderManager::addShader(const std::string &name,
                              std::shared_ptr<Shader> shader) {
  if (m_shaders.contains(name)) {
    throw std::runtime_error(
        std::format("A shader with name {} already exists", name));
  }

  m_shaders[name] = std::move(shader);
}

std::shared_ptr<Shader>
ShaderManager::getShader(const std::string &name) const {
  if (!m_shaders.contains(name)) {
    std::cout << std::format("No shader with name {} exists", name)
              << std::endl;

    return {};
  }

  return m_shaders.at(name);
}

} // namespace libs::renderer