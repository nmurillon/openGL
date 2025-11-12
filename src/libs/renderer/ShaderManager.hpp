#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Shader.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT ShaderManager {
public:
  ShaderManager();
  ~ShaderManager() = default;

  std::shared_ptr<Shader> getShader(const std::string &name) const;
  void addShader(const std::string &name, std::shared_ptr<Shader> shader);
  void addShader(const std::string &name, const std::string &vertexSrcFile,
                 const std::string &fragmentSrcFile);

  void addSearchPath(const std::string &path);
  void addSearchPath(const std::filesystem::path &path);

private:
  /**
   * This function searches for the first path where the given file exists
   * among the registered search paths. If none is found, it returns the input
   * path.
   * @param [in] fileName The name of the file to search for
   * @return The first matching path or the input path if none found
   */
  std::filesystem::path getFirstMatchingPath(const std::string &fileName) const;

  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
  std::vector<std::filesystem::path> m_searchPaths;

  // TODO: add search path for shaders
};
} // namespace libs::renderer