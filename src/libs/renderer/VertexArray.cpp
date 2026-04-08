#include <libs/renderer/VertexArray.hpp>

#include <libs/renderer/Vertex.hpp>

namespace libs::renderer {
VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::VertexArray(VertexBuffer &&vertexBuffer) : VertexArray() {
  setVertexBuffer(std::move(vertexBuffer));
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id(other.m_id), m_vertexBuffer(std::move(other.m_vertexBuffer)),
      m_indexBuffer(std::move(other.m_indexBuffer)) {

  other.m_id = 0;
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (m_id) {
    glDeleteVertexArrays(1, &m_id);
  }

  m_id = other.m_id;
  m_vertexBuffer = std::move(other.m_vertexBuffer);
  m_indexBuffer = std::move(other.m_indexBuffer);

  other.m_id = 0;

  return *this;
}

VertexArray::~VertexArray() {
  if (m_id) {
    glDeleteVertexArrays(1, &m_id);
  }
}

void VertexArray::setVertexBuffer(VertexBuffer &&vertexBuffer) {
  m_vertexBuffer = std::move(vertexBuffer);
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

  vertexBuffer.unbind();
  unbind();
}

void VertexArray::setIndexBuffer(IndexBuffer &&indexBuffer) {
  m_indexBuffer = std::move(indexBuffer);

  bind();
  m_indexBuffer.bind();

  unbind();
  m_indexBuffer.unbind();
}

const VertexBuffer &VertexArray::getVertexBuffer() const {
  return m_vertexBuffer;
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace libs::renderer