#include "GeometryShaderLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Geometry shader sandbox"};

  app.addLayer(std::make_shared<GeometryShaderLayer>());

  app.run();

  return 0;
}