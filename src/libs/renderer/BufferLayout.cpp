#include <libs/renderer/BufferLayout.hpp>

namespace libs::renderer {

BufferLayout::BufferLayout() {}

BufferLayout::BufferLayout(std::initializer_list<BufferLayoutElement> elements)
    : m_elements(elements) {
  for (auto &element : m_elements) {
    element.setOffset(m_stride);
    m_stride += element.getSize() * element.getCount();
  }
}

GLsizei BufferLayout::getStride() const { return m_stride; }

BufferLayout::BufferLayoutElements BufferLayout::getElements() const {
  return m_elements;
}

} // namespace libs::renderer