#include "MultiLightsLayer.hpp"
#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"OpenGL - Multi Lights chapter"};

  app.addLayer(std::make_shared<MultiLightsLayer>());

  app.run();

  return 0;
}