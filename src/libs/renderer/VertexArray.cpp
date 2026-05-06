#include <libs/renderer/VertexArray.hpp>

#include <libs/renderer/Vertex.hpp>

namespace libs::renderer {
VertexArray::VertexArray() { glGenVertexArrays(1, &m_id); }

VertexArray::VertexArray(VertexBuffer &&vertexBuffer) : VertexArray() {
  addVertexBuffer(std::move(vertexBuffer));
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id(other.m_id), m_buffers(std::move(other.m_buffers)),
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
  m_buffers = std::move(other.m_buffers);
  m_indexBuffer = std::move(other.m_indexBuffer);

  other.m_id = 0;

  return *this;
}

VertexArray::~VertexArray() {
  if (m_id) {
    glDeleteVertexArrays(1, &m_id);
  }
}

void VertexArray::setIndexBuffer(IndexBuffer &&indexBuffer) {
  m_indexBuffer = std::move(indexBuffer);

  bind();
  m_indexBuffer.bind();

  unbind();
  m_indexBuffer.unbind();
}

void VertexArray::addVertexBuffer(VertexBuffer &&vertexBuffer) {
  bind();
  vertexBuffer.bind();

  const auto layout = vertexBuffer.getLayout();
  const auto elements = vertexBuffer.getLayout().getElements();
  for (std::size_t i = 0; i < elements.size(); i++) {
    const auto &element = elements.at(i);
    // TODO: find a way to disable attribute if not used.
    glEnableVertexAttribArray(i + m_attributeOffset);
    glVertexAttribPointer(i + m_attributeOffset, element.getCount(),
                          element.getType(), element.isNormalized(),
                          layout.getStride(),
                          reinterpret_cast<void *>(element.getOffset()));

    glVertexAttribDivisor(i + m_attributeOffset, element.getAttribDivisor());
  }

  m_attributeOffset += elements.size();

  vertexBuffer.unbind();
  unbind();

  m_buffers.push_back(std::move(vertexBuffer));
}

int VertexArray::getVertexCount() const {
  if (m_buffers.empty()) {
    return 0;
  }

  return m_buffers.at(0).getVertexCount();
}

void VertexArray::bind() const { glBindVertexArray(m_id); }

void VertexArray::unbind() { glBindVertexArray(0); }

} // namespace libs::renderer