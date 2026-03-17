#pragma once

#include <libs/openGl/opengl.h>
#include <libs/renderer/Buffer.hpp>


#include <vector>

namespace libs::renderer {

class BufferLayoutElement {
public:
  BufferLayoutElement(int size, int type = GL_FLOAT, int normalized = GL_FALSE,
                      int stride = 0, int offset = 0)
      : m_size(size), m_type(type), m_normalized(normalized), m_stride(stride),
        m_offset(offset) {}
  unsigned int getSize() const { return m_size; }

private:
  int m_size;
  int m_type;
  int m_normalized;
  int m_stride;
  int m_offset;
};

class BufferLayout {
public:
  BufferLayout(std::vector<BufferLayoutElement> elements)
      : m_elements(elements) {}
  virtual ~BufferLayout() = default;

  virtual std::vector<unsigned int> getAttributeSizes() const = 0;
  virtual unsigned int getStride() const = 0;

private:
  std::vector<BufferLayoutElement> m_elements;
};
} // namespace libs::renderer