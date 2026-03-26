#include <libs/renderer/Cube.hpp>

#include <libs/openGl/opengl.h>

#include <libs/renderer/IndexBuffer.hpp>
#include <libs/renderer/VertexBuffer.hpp>

namespace libs::renderer {
Cube::Cube(BufferLayout &&layout, const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices)
    : m_vertices(vertices), m_indices(indices) {

  VertexBuffer vertexBuffer{std::move(layout), vertices.data(),
                            vertices.size() * sizeof(float)};

  m_vertexArray.setVertexBuffer(std::forward<VertexBuffer>(vertexBuffer));

  if (m_indices.size()) {
    IndexBuffer indexBuffer{m_indices.data(),
                            m_indices.size() * sizeof(unsigned int)};
    m_vertexArray.setIndexBuffer(std::forward<IndexBuffer>(indexBuffer));
  }
}

void Cube::draw() const {
  m_vertexArray.bind();
  if (!m_indices.empty()) {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
                   GL_UNSIGNED_INT, 0);
  } else {
    const auto count = m_vertexArray.getVertexBuffer().getVertexCount();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
  }
  m_vertexArray.unbind();
}

}; // namespace libs::renderer