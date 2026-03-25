#pragma once

#include <logl/renderer/export.h>

#include <libs/renderer/Buffer.hpp>
#include <libs/renderer/IndexBuffer.hpp>
#include <libs/renderer/VertexBuffer.hpp>

namespace libs::renderer {
class LOGL_RENDERER_EXPORT VertexArray : public Buffer {
public:
  VertexArray();
  VertexArray(VertexBuffer &&vertexBuffer);
  virtual ~VertexArray() = default;

  void setVertexBuffer(const VertexBuffer &vertexBuffer);
  const VertexBuffer &getVertexBuffer() const;
  void setData(const void *data, std::size_t size);

  void setIndexBuffer(const IndexBuffer &indexBuffer);

  void bind() const override;
  void unbind() const override;

private:
  // TODO: we might have mltiple buffers in the future
  VertexBuffer m_vertexBuffer;
  IndexBuffer m_indexBuffer;
};
} // namespace libs::renderer