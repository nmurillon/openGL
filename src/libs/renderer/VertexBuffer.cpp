#include <libs/renderer/VertexBuffer.hpp>

namespace libs::renderer {
VertexBuffer::VertexBuffer() { glGenBuffers(1, &m_id); };

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
} // namespace libs::renderer