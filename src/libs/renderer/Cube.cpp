#include <libs/renderer/Cube.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

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

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(0));

  // Vertex attribute 1
  if (m_type != DataType::POSITION) {
    const auto size = (m_type == DataType::POSITION_TEXTURE) ? 2 : 3;

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(3 * sizeof(float)));
  }

  // Vertex attribute 2
  if (m_type == DataType::POSITION_NORMAL_TEXTURE) {
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(6 * sizeof(float)));
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
  switch (m_type) {
  case DataType::POSITION:
    return 3;
  case DataType::POSITION_COLOR:
    return 6;
  case DataType::POSITION_TEXTURE:
    return 5;
  case DataType::POSITION_NORMAL:
    return 6;
  case DataType::POSITION_NORMAL_TEXTURE:
    return 8;
  default:
    return 0;
  }
}

int Cube::getStride() const { return getVertexOffset() * sizeof(float); }
}; // namespace libs::renderer