#include "application/MaterialsLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Materials chapter"};

  app.addLayer(std::make_shared<MaterialsLayer>());

  app.run();

  return 0;
}