#include <libs/renderer/ShaderManager.hpp>

#include <format>
#include <iostream>

namespace libs::renderer {
ShaderManager::ShaderManager()
    : m_commonShaderDirectory(
          libs::io::ProgramPath::getInstance().getProgramDir() /
          LOGL_RENDERER_RESOURCES_FOLDER / "shaders") {

  addShader("loglBasicShader", m_commonShaderDirectory / "basicShader.vert",
            m_commonShaderDirectory / "basicShader.frag");
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

void ShaderManager::addShader(const std::string &name,
                              std::shared_ptr<Shader> shader) {
  if (m_shaders.contains(name)) {
    throw std::runtime_error(
        std::format("A shader with name {} already exists", name));
  }

  m_shaders[name] = std::move(shader);
}

void ShaderManager::addShader(const std::string &name,
                              const std::string &vertexSrcFile,
                              const std::string &fragmentSrcFile) {
  if (m_shaders.contains(name)) {
    throw std::runtime_error(
        std::format("A shader with name {} already exists", name));
  }

  m_shaders[name] = std::make_shared<Shader>(vertexSrcFile, fragmentSrcFile);
}

std::filesystem::path ShaderManager::getCommonShaderDirectory() const {
  return m_commonShaderDirectory;
}

} // namespace libs::renderer