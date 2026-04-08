#pragma once

#include <logl/renderer/export.h>

#include <libs/openGl/opengl.h>

#include <cstddef>
#include <vector>

namespace libs::renderer {

class LOGL_RENDERER_EXPORT BufferLayoutElement {
public:
  // TODO: in the future we might need to set the offset explicitly
  BufferLayoutElement(std::size_t size, GLint count, GLenum type = GL_FLOAT,
                      bool normalized = GL_FALSE)
      : m_size(size), m_count(count), m_type(type),
        m_normalized(normalized ? GL_TRUE : GL_FALSE) {}
  ~BufferLayoutElement() = default;

  unsigned int getSize() const { return m_size; }
  GLint getCount() const { return m_count; }
  GLenum getType() const { return m_type; }
  GLboolean isNormalized() const { return m_normalized; }
  unsigned long getOffset() const { return m_offset; }
  void setOffset(unsigned long offset) { m_offset = offset; }

private:
  std::size_t m_size{0};
  GLint m_count{0};
  GLenum m_type{GL_FLOAT};
  GLboolean m_normalized{GL_FALSE};
  unsigned long m_offset{0};
};

class LOGL_RENDERER_EXPORT BufferLayout {
public:
  using BufferLayoutElements = std::vector<BufferLayoutElement>;

  BufferLayout();
  BufferLayout(std::initializer_list<BufferLayoutElement> elements);
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