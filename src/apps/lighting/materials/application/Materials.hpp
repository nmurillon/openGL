#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float shininess;
};

struct Light {
  glm::vec3 position;
  glm::vec3 color;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

const Light defaultLight{
    glm::vec3(1.2f, 1.0f, 2.0f),
    glm::vec3(1.f, 1.f, 1.f),
    glm::vec3(0.2f),
    glm::vec3(0.5f),
    glm::vec3(1.f),
};

const Material tutorial{glm::vec3{1.0, 0.5, 0.31}, glm::vec3{1.0, 0.5, 0.31},
                        glm::vec3{0.5, 0.5, 0.5}, 32.0f};

const Material emerald{glm::vec3{0.0215, 0.1745, 0.0215},
                       glm::vec3{0.07568, 0.61424, 0.07568},
                       glm::vec3{0.633, 0.727811, 0.633}, 128.0 * 0.6};

const Material jade{glm::vec3{0.135, 0.2225, 0.1575},
                    glm::vec3{0.54, 0.89, 0.63},
                    glm::vec3{0.316228, 0.316228, 0.316228}, 128.0 * 0.1};

const Material yellowPlastic{glm::vec3(0.f), glm::vec3{0.5, 0.5, 0.},
                             glm::vec3{0.6, 0.6, 0.5}, 128.0 * 0.25};

inline std::vector<std::string> getAllMaterialTypeNames() {
  return {"Tutorial", "Emerald", "Jade", "Yellow Plastic"};
}

inline Material getMaterialByString(const std::string &name) {
  if (name == "Tutorial") {
    return tutorial;
  } else if (name == "Emerald") {
    return emerald;
  } else if (name == "Jade") {
    return jade;
  } else if (name == "Yellow Plastic") {
    return yellowPlastic;
  }

  return tutorial;
}