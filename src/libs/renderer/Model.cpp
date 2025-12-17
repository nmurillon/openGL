#include <libs/renderer/Model.hpp>

#include <iostream>

namespace {
glm::vec3 toVec3(const aiVector3D &vec) {
  return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec2 toVec2(const aiVector3D &vec) { return glm::vec2(vec.x, vec.y); }
} // namespace

namespace libs::renderer {
Model::Model(const std::string &path)
    : m_directory(std::filesystem::path{path}.parent_path()) {
  load(path);
}

void Model::draw(Shader &shader) const {
  for (auto &mesh : m_meshes) {
    mesh.draw(shader);
  }
}

bool Model::isValid() const { return m_isValid; }

void Model::load(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }
  m_directory = std::filesystem::path(path).parent_path();

  processNode(scene->mRootNode, scene);
  m_isValid = true;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene);
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  // process vertex positions, normals and texture coordinates
  vertices.reserve(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    const auto position = toVec3(mesh->mVertices[i]);

    glm::vec3 normal{0.f};
    if (mesh->HasNormals()) {
      normal = toVec3(mesh->mNormals[i]);
    }

    glm::vec2 texCoords{0.f};
    if (mesh->mTextureCoords[0]) {
      texCoords = toVec2(mesh->mTextureCoords[0][i]);
    }

    vertices.emplace_back(position, normal, texCoords);
  }

  // process indices
  indices.reserve(mesh->mNumFaces *
                  3); // 3 because aiProcess_Triangulate is used
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.emplace_back(face.mIndices[j]);
  }

  // process material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE,
                         textures);
    loadMaterialTextures(material, aiTextureType_SPECULAR,
                         TextureType::SPECULAR, textures);
  }

  m_meshes.emplace_back(vertices, indices, textures);
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                 TextureType textureType,
                                 std::vector<Texture> &textures) {
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    textures.emplace_back(textureType, m_directory / str.C_Str());
  }
}
} // namespace libs::renderer