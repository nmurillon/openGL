#include <libs/renderer/ShaderManager.hpp>

#include <libs/io/ProgramPath.hpp>

#include <format>
#include <iostream>

namespace libs::renderer {
ShaderManager::ShaderManager() {
  m_searchPaths.emplace_back(
      libs::io::ProgramPath::getInstance().getProgramDir() /
      LOGL_RENDERER_RESOURCES_FOLDER / "shaders");

  addShader("loglBasicShader", "basicShader.vert", "basicShader.frag");
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

  const auto vertexPath = getFirstMatchingPath(vertexSrcFile).string();
  const auto fragmentPath = getFirstMatchingPath(fragmentSrcFile).string();

  m_shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath);
}

void ShaderManager::addSearchPath(const std::string &path) {
  m_searchPaths.emplace_back(path);
}

void ShaderManager::addSearchPath(const std::filesystem::path &path) {
  m_searchPaths.emplace_back(path);
}

std::filesystem::path
ShaderManager::getFirstMatchingPath(const std::string &filePath) const {

  if (std::filesystem::exists(filePath)) {
    return {filePath};
  }

  for (const auto &searchPath : m_searchPaths) {
    const std::filesystem::path fullPath = searchPath / filePath;
    if (std::filesystem::exists(fullPath)) {
      return fullPath;
    }
  }

  return {filePath};
}

} // namespace libs::renderer