#include <libs/renderer/VertexBuffer.hpp>

#include <utility>

namespace libs::renderer {
VertexBuffer::VertexBuffer() { glGenBuffers(1, &m_id); }

VertexBuffer::VertexBuffer(BufferLayout &&bufferLayout)
    : m_layout(bufferLayout) {
  glGenBuffers(1, &m_id);
};

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::setLayout(BufferLayout &&layout) {
  m_layout = std::move(layout);
}

const BufferLayout &VertexBuffer::getLayout() const { return m_layout; }

void VertexBuffer::setData(const void *data, std::size_t size, GLenum usage) {
  m_data = data;
  bind();

  glBufferData(GL_ARRAY_BUFFER, size, m_data, usage);
}
} // namespace libs::renderer