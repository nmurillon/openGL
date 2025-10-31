#include "application/ColorAppLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Colors chapter"};

  app.addLayer(std::make_shared<ColorAppLayer>());

  app.run();

  return 0;
}