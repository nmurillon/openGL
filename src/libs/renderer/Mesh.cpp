#include <libs/renderer/Mesh.hpp>

#include <libs/openGl/opengl.h>

#include <format>
#include <string>

namespace libs::renderer {
Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Texture> &textures)
    : vertices(vertices), indices(indices), textures(textures) {
  init();
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

  glBindVertexArray(m_vao);

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void Mesh::init() {
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
               indices.data(), GL_STATIC_DRAW);

  const auto stride = sizeof(Vertex);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(0));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(offsetof(Vertex, normal)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(offsetof(Vertex, texCoord)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace libs::renderer