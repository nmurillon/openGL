#include "application/AugmentedLogoLayer.hpp"

#include <libs/core/Application.hpp>

#include <memory>

int main(int argc, char **argv) {
  libs::core::Application app{"Augmented Logo Application"};
  app.addLayer(std::make_shared<AugmentedLogoLayer>());

  app.run();
  return 0;
}