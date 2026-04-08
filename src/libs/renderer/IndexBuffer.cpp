#include <libs/renderer/IndexBuffer.hpp>

namespace libs::renderer {
IndexBuffer::IndexBuffer() { glGenBuffers(1, &m_id); }

IndexBuffer::IndexBuffer(const void *data, std::size_t size, GLenum usage) {
  glGenBuffers(1, &m_id);
  setData(data, size, usage);
  unbind();
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    : m_id(other.m_id), m_data(other.m_data) {

  other.m_id = 0;
  other.m_data = nullptr;
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  m_id = other.m_id;
  m_data = other.m_data;

  return *this;
}

IndexBuffer::~IndexBuffer() {
  if (m_id) {
    glDeleteBuffers(1, &m_id);
  }
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::setData(const void *data, std::size_t size, GLenum usage) {
  m_data = data;
  bind();

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), m_data,
               usage);
}
} // namespace libs::renderer