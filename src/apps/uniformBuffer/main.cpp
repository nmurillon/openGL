#include "UniformBufferLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Uniform buffer sandbox"};

  app.addLayer(std::make_shared<UniformBufferLayer>());

  app.run();

  return 0;
}