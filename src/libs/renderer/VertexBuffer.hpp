#pragma once

#include <libs/renderer/Buffer.hpp>

namespace libs::renderer {
class VertexBuffer : public Buffer {
public:
  VertexBuffer();
  virtual ~VertexBuffer() = default;

  void bind() const override;
  void unbind() const override;
};
} // namespace libs::renderer