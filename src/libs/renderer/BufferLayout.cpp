#include <libs/renderer/BufferLayout.hpp>

#include <algorithm>
#include <stdexcept>

namespace libs::renderer {

BufferLayout::BufferLayout() {}

BufferLayout::BufferLayout(std::initializer_list<BufferLayoutElement> elements)
    : m_elements(elements) {
  for (auto &element : m_elements) {
    element.setOffset(m_stride);
    m_stride += element.getSize() * element.getCount();
  }
}

BufferLayout::BufferLayout(int stride,
                           std::vector<BufferLayoutElement> elements)
    : m_elements(elements), m_stride(stride) {

  auto p = [&](const BufferLayoutElement &e) { return e.getOffset() == 0; };

  if (std::any_of(m_elements.begin() + 1, m_elements.end(), p)) {
    throw std::runtime_error(
        "When creating a buffer layout by explicitly giving, each elememt "
        "should explicitly init its offset");
  }
}

GLsizei BufferLayout::getStride() const { return m_stride; }

BufferLayout::BufferLayoutElements BufferLayout::getElements() const {
  return m_elements;
}

} // namespace libs::renderer