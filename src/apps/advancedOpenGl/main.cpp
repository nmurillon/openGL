#include "AdvancedOpenGlLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Model Loading sandbox"};

  app.addLayer(std::make_shared<AdvancedOpenGlLayer>());

  app.run();

  return 0;
}