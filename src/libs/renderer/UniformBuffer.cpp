#include <libs/renderer/UniformBuffer.hpp>

namespace libs::renderer {
UniformBuffer::UniformBuffer(int size) : m_size(size) {
  glGenBuffers(1, &m_id);
  bind();
  glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, GL_STATIC_DRAW);
  unbind();
}

void UniformBuffer::bind() const { glBindBuffer(GL_UNIFORM_BUFFER, m_id); }

void UniformBuffer::unbind() { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

void UniformBuffer::setBindingPoint(int bindingPoint) const {
  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_id);
}

void UniformBuffer::setData(int offset, int size, const void *data) const {
  bind();
  glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
  unbind();
}
} // namespace libs::renderer