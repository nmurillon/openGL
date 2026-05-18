#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

#include <cstddef>
#include <vector>

namespace libs::renderer {

struct LOGL_RENDERER_EXPORT BufferLayoutElement {
public:
  unsigned int getSize() const { return m_size; }
  GLint getCount() const { return m_count; }
  GLenum getType() const { return m_type; }
  GLboolean isNormalized() const { return m_normalized; }
  unsigned long getOffset() const { return m_offset; }
  void setOffset(unsigned long offset) { m_offset = offset; }
  unsigned int getAttribDivisor() const { return m_attribDivisor; }

  std::size_t m_size{0};
  GLint m_count{0};
  GLenum m_type{GL_FLOAT};
  GLboolean m_normalized{GL_FALSE};
  unsigned long m_offset{0};
  unsigned int m_attribDivisor{0};
};

class LOGL_RENDERER_EXPORT BufferLayout {
public:
  using BufferLayoutElements = std::vector<BufferLayoutElement>;

  BufferLayout();
  BufferLayout(std::initializer_list<BufferLayoutElement> elements);
  BufferLayout(int stride, std::vector<BufferLayoutElement> elements);
  BufferLayout(BufferLayout &&other) = default;
  BufferLayout(const BufferLayout &other) = default;
  BufferLayout &operator=(BufferLayout &&other) = default;
  BufferLayout &operator=(const BufferLayout &other) = default;

  ~BufferLayout() = default;

  GLsizei getStride() const;
  BufferLayoutElements getElements() const;

private:
  GLsizei m_stride{};
  BufferLayoutElements m_elements{};
};
} // namespace libs::renderer