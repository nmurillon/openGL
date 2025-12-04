#include "application/LightCasterLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Light Casters chapter"};

  app.addLayer(std::make_shared<LightCasterLayer>());

  app.run();

  return 0;
}