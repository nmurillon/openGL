#include <libs/renderer/VertexBuffer.hpp>

#include <utility>

namespace libs::renderer {
VertexBuffer::VertexBuffer() { glGenBuffers(1, &m_id); }

VertexBuffer::VertexBuffer(BufferLayout &&bufferLayout)
    : m_layout(bufferLayout) {
  glGenBuffers(1, &m_id);
};

VertexBuffer::VertexBuffer(BufferLayout &&layout, const void *data,
                           std::size_t size, GLenum usage)
    : m_layout(layout) {
  glGenBuffers(1, &m_id);
  setData(data, size, usage);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept
    : m_id(other.m_id), m_layout(std::move(other.m_layout)),
      m_data(other.m_data) {

  other.m_id = 0;
  other.m_data = nullptr;
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  m_id = other.m_id;
  m_data = other.m_data;
  m_layout = std::move(other.m_layout);

  return *this;
}

VertexBuffer::~VertexBuffer() {
  if (m_id) {
    glDeleteBuffers(1, &m_id);
  }
}

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