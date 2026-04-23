#include "InstancingLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Instancing sandbox"};

  app.addLayer(std::make_shared<InstancingLayer>());

  app.run();

  return 0;
}