#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Buffer.hpp>
#include <libs/renderer/BufferLayout.hpp>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT VertexBuffer : public Buffer {
public:
  VertexBuffer();
  VertexBuffer(BufferLayout &&layout);
  // TODO: see to create buffer with data in the constructor
  virtual ~VertexBuffer() = default;

  void bind() const override;
  void unbind() const override;

  void setLayout(BufferLayout &&layout);
  const BufferLayout &getLayout() const;
  void setData(const void *data, std::size_t size,
               GLenum usage = GL_STATIC_DRAW);

private:
  BufferLayout m_layout;
  const void *m_data{nullptr};
};
} // namespace libs::renderer