#include <libs/renderer/IndexBuffer.hpp>

namespace libs::renderer {
IndexBuffer::IndexBuffer() { glGenBuffers(1, &m_id); }

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::setData(const void *data, std::size_t size, GLenum usage) {
  m_data = data;
  bind();

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), m_data,
               usage);
}
} // namespace libs::renderer