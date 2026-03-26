#include <libs/renderer/Mesh.hpp>

#include <libs/openGl/opengl.h>
#include <libs/renderer/IndexBuffer.hpp>
#include <libs/renderer/VertexBuffer.hpp>

#include <format>
#include <string>

namespace libs::renderer {
Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices,
           std::vector<Texture> &&textures)
    : vertices(std::move(vertices)), indices(std::move(indices)),
      textures(std::move(textures)) {
  init();
}

Mesh::Mesh(Mesh &&other) noexcept
    : vertices(std::move(other.vertices)), indices(std::move(other.indices)),
      textures(std::move(other.textures)), m_vao(std::move(other.m_vao)) {}

Mesh &Mesh::operator=(Mesh &&other) {
  if (this == &other) {
    return *this;
  }

  vertices = std::move(other.vertices);
  indices = std::move(other.indices);
  textures = std::move(other.textures);
  m_vao = std::move(other.m_vao);

  return *this;
}

void Mesh::draw(Shader &shader) const {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    const auto type = textures[i].type();
    unsigned int number;
    if (type == TextureType::DIFFUSE) {
      number = diffuseNr++;
    } else if (type == TextureType::SPECULAR) {
      number = specularNr++;
    }

    textureTypeToString(textures[i].type());
    shader.setInt(
        std::format("material.{}{}", textureTypeToString(type), number).c_str(),
        i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id());
  }
  shader.setFloat("material.shininess", 16.f);
  glActiveTexture(GL_TEXTURE0);

  m_vao.bind();

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);

  m_vao.unbind();
}

void Mesh::init() {
  VertexBuffer vbo{BufferLayout{
                       BufferLayoutElement(sizeof(float), 3, GL_FLOAT),
                       BufferLayoutElement(sizeof(float), 3, GL_FLOAT),
                       BufferLayoutElement(sizeof(float), 2, GL_FLOAT),
                   },
                   vertices.data(), vertices.size() * sizeof(Vertex),
                   GL_STATIC_DRAW};

  IndexBuffer indexBuffer{indices.data(), indices.size() * sizeof(unsigned int),
                          GL_STATIC_DRAW};

  m_vao.setVertexBuffer(std::forward<VertexBuffer>(vbo));
  m_vao.setIndexBuffer(std::forward<IndexBuffer>(indexBuffer));
}
} // namespace libs::renderer