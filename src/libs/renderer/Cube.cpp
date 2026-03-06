#include <libs/renderer/Cube.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <numeric>

namespace libs::renderer {
Cube::Cube(const DataType &type, const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices)
    : m_type(type), m_vertices(vertices), m_indices(indices) {

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
               m_vertices.data(), GL_STATIC_DRAW);

  if (!m_indices.empty()) {
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_indices.size() * sizeof(unsigned int), m_indices.data(),
                 GL_STATIC_DRAW);
  }

  const auto stride = getStride();

  const auto sizes = getAttributeSizes();
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, sizes[0], GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(0));

  // Vertex attribute 1
  if (sizes.size() > 1) {
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizes[1], GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(sizes[0] * sizeof(float)));
  }

  // Vertex attribute 2
  if (sizes.size() > 2) {
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, sizes[2], GL_FLOAT, GL_FALSE, stride,
        reinterpret_cast<void *>((sizes[1] + sizes[0]) * sizeof(float)));
  }

  // Unbind VBO & VAO to prevent further operations to modify them
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Cube::draw() const {
  glBindVertexArray(m_VAO);
  if (!m_indices.empty()) {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                   GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0,
                 static_cast<GLsizei>(m_vertices.size() / getVertexOffset()));
  }
}

int Cube::getVertexOffset() const {
  const auto sizes = getAttributeSizes();
  return std::accumulate(sizes.begin(), sizes.end(), 0);
}

int Cube::getStride() const { return getVertexOffset() * sizeof(float); }

std::vector<int> Cube::getAttributeSizes() const {
  switch (m_type) {
  case DataType::POSITION:
    return {3};
  case DataType::POSITION_COLOR:
    return {3, 3};
  case DataType::POSITION_TEXTURE:
    return {3, 2};
  case DataType::POSITION2D_TEXTURE:
    return {2, 2};
  case DataType::POSITION_NORMAL:
    return {3, 3};
  case DataType::POSITION_NORMAL_TEXTURE:
    return {3, 3, 2};
  }
}

}; // namespace libs::renderer