#pragma once

#include <glm/glm.hpp>

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float shininess;
};

const Material emerald{glm::vec3{0.0215, 0.1745, 0.0215},
                       glm::vec3{0.07568, 0.61424, 0.07568},
                       glm::vec3{0.633, 0.727811, 0.633}, 128.0 * 0.6};

const Material jade{glm::vec3{0.135, 0.2225, 0.1575},
                    glm::vec3{0.54, 0.89, 0.63},
                    glm::vec3{0.316228, 0.316228, 0.316228}, 128.0 * 0.1};

const Material yellowPlastic{glm::vec3(0.f), glm::vec3{0.5, 0.5, 0.},
                             glm::vec3{0.6, 0.6, 0.5}, 128.0 * 0.25};