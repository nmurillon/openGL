#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Mesh.hpp>
#include <libs/renderer/Shader.hpp>
#include <libs/renderer/Texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>
#include <string>
#include <vector>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT Model {
public:
  Model(const std::string &path);

  void draw(Shader &shader) const;

private:
  void load(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  void processMesh(aiMesh *mesh, const aiScene *scene);
  void loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                            TextureType typeName,
                            std::vector<Texture> &textures);

  std::vector<Mesh> m_meshes;
  std::string m_name;
  std::filesystem::path m_directory;
};
} // namespace libs::renderer