#include <libs/renderer/VertexArray.hpp>

#include <libs/renderer/Vertex.hpp>

namespace libs::renderer {
VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::VertexArray(VertexBuffer &&vertexBuffer) : VertexArray() {
  setVertexBuffer(vertexBuffer);
}

void VertexArray::setVertexBuffer(const VertexBuffer &vertexBuffer) {
  m_vertexBuffer = vertexBuffer;
  bind();
  vertexBuffer.bind();

  const auto layout = m_vertexBuffer.getLayout();
  const auto elements = m_vertexBuffer.getLayout().getElements();
  for (std::size_t i = 0; i < elements.size(); i++) {
    const auto &element = elements.at(i);
    // TODO: find a way to disable attribute if not used.
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.getCount(), element.getType(),
                          element.isNormalized(), layout.getStride(),
                          reinterpret_cast<void *>(element.getOffset()));
  }
}

const VertexBuffer &VertexArray::getVertexBuffer() const {
  return m_vertexBuffer;
}

void VertexArray::setData(const void *data, std::size_t size) {
  m_vertexBuffer.setData(data, size);

  bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() const {
  m_vertexBuffer.unbind();
  glBindVertexArray(0);
}

} // namespace libs::renderer