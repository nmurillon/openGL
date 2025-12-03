#pragma once

#include <vector>

namespace mesh::lighting {
// clang-format off

  // Position as a vec3 and texture as vec2
  // For the texture, we need to duplicate the vertices
  const std::vector<float> verticesWithTexture = {
    // Up face
    -0.5f,  0.5f,  0.5f, 0.f, 1.f, // TOP-LEFT-UP
     0.5f,  0.5f,  0.5f, 1.f, 1.f, // TOP-RIGHT-UP
   -0.5f,-0.5f, 0.5f,0.f,0.f, // BOTTOM-LEFT-UP
    0.5f,-0.5f, 0.5f,1.f,0.f,// BOTTOM-RIGHT-UP
  // Left face
  -0.5f,  0.5f, 0.5f, 0.f, 1.f, // TOP-LEFT-UP
  -0.5f, -0.5f, 0.5f, 1.f, 1.f, // BOTTOM-LEFT-UP
  -0.5f,  0.5f,-0.5f, 0.f, 0.f,// TOP-LEFT-DOWN
  -0.5f, -0.5f,-0.5f, 1.f, 0.f,// BOTTOM-LEFT-DOWN
  // Far face 
  -0.5f,  0.5f, 0.5f, 1.f, 1.f, // TOP-LEFT-UP
   0.5f,  0.5f, 0.5f, 0.f, 1.f, // TOP-RIGHT-UP
  -0.5f,  0.5f,-0.5f, 1.f, 0.f,// TOP-LEFT-DOWN
   0.5f,  0.5f,-0.5f, 0.f, 0.f,// TOP-RIGHT-DOWN
  // Right face
 0.5f,  0.5f,  0.5f, 1.f, 1.f, // TOP-RIGHT-UP
 0.5f, -0.5f,  0.5f, 0.f, 1.f,// BOTTOM-RIGHT-UP
 0.5f,  0.5f, -0.5f, 1.f, 0.f,// TOP-RIGHT-DOWN
 0.5f, -0.5f, -0.5f, 0.f, 0.f,// BOTTOM-RIGHT-DOWN
  // Near face
  -0.5f, -0.5f,  0.5f, 0.f, 1.f, // BOTTOM-LEFT-UP
   0.5f, -0.5f,  0.5f, 1.f, 1.f,// BOTTOM-RIGHT-UP
  -0.5f, -0.5f, -0.5f, 0.f, 0.f,// BOTTOM-LEFT-DOWN
   0.5f, -0.5f, -0.5f, 1.f, 0.f,// BOTTOM-RIGHT-DOWN
  // Down face
  -0.5f,  0.5f, -0.5f, 0.f, 1.f,// TOP-LEFT-DOWN
   0.5f,  0.5f, -0.5f, 1.f, 1.f,// TOP-RIGHT-DOWN
  -0.5f, -0.5f, -0.5f, 0.f, 0.f,// BOTTOM-LEFT-DOWN
   0.5f, -0.5f, -0.5f, 1.f, 0.f,// BOTTOM-RIGHT-DOWN
  };

  const std::vector<unsigned int> indicesForTexture = {
    // Up face
    0,1,2,
    1,2,3,
    // Left face
    4,5,6,
    5,6,7,
    // Far face
    8,9,10,
    9,10,11,
    // Right face
    12,13,14,
    13,14,15,
    // Near face
    16,17,18,
    17,18,19,
    // Down face
    20,21,22,
    21,22,23,
  };

  // Position as a vec3 and color as vec3
  // For the texture, we need to duplicate the vertices
  const std::vector<float> verticesWithColors = {
    -0.5f,  0.5f,  0.5f, 1.f, 0.5f, 0.31f, // TOP-LEFT-UP
     0.5f,  0.5f,  0.5f, 1.f, 0.5f, 0.31f, // TOP-RIGHT-UP
    -0.5f, -0.5f,  0.5f, 1.f, 0.5f, 0.31f, // BOTTOM-LEFT-UP
     0.5f, -0.5f,  0.5f, 1.f, 0.5f, 0.31f, // BOTTOM-RIGHT-UP
    -0.5f,  0.5f, -0.5f, 1.f, 0.5f, 0.31f, // TOP-LEFT-DOWN
     0.5f,  0.5f, -0.5f, 1.f, 0.5f, 0.31f, // TOP-RIGHT-DOWN
    -0.5f, -0.5f, -0.5f, 1.f, 0.5f, 0.31f, // BOTTOM-LEFT-DOWN
     0.5f, -0.5f, -0.5f, 1.f, 0.5f, 0.31f, // BOTTOM-RIGHT-DOWN
  };

  const std::vector<unsigned int> indicesForColors = {
    // Up face
    0,1,2,
    1,2,3,
    // Left face
    0,2,4,
    2,4,6,
    // Far face
    0,1,4,
    1,4,5,
    // Right face
    1,3,5,
    3,5,7,
    // Near face
    2,3,6,
    3,6,7,
    // Down face
    4,5,6,
    5,6,7,
  };

  namespace withNormals {
  // Position as a vec3 and normals as vec3
  const std::vector<float> vertices = {
    // Up face
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, // TOP-LEFT-UP
     0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, // TOP-RIGHT-UP
   -0.5f,-0.5f, 0.5f,0.f, 0.f, 1.f, // BOTTOM-LEFT-UP
    0.5f,-0.5f, 0.5f,0.f, 0.f, 1.f,// BOTTOM-RIGHT-UP
  // Left face
  -0.5f,  0.5f, 0.5f, -1.f, 0.f, 0.f, // TOP-LEFT-UP
  -0.5f, -0.5f, 0.5f, -1.f, 0.f, 0.f, // BOTTOM-LEFT-UP
  -0.5f,  0.5f,-0.5f, -1.f, 0.f, 0.f,// TOP-LEFT-DOWN
  -0.5f, -0.5f,-0.5f, -1.f, 0.f, 0.f,// BOTTOM-LEFT-DOWN
  // Far face 
  -0.5f,  0.5f, 0.5f, 0.f, 1.f, 0.f, // TOP-LEFT-UP
   0.5f,  0.5f, 0.5f, 0.f, 1.f, 0.f, // TOP-RIGHT-UP
  -0.5f,  0.5f,-0.5f, 0.f, 1.f, 0.f,// TOP-LEFT-DOWN
   0.5f,  0.5f,-0.5f, 0.f, 1.f, 0.f,// TOP-RIGHT-DOWN
  // Right face
 0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f, // TOP-RIGHT-UP
 0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f,// BOTTOM-RIGHT-UP
 0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f,// TOP-RIGHT-DOWN
 0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f,// BOTTOM-RIGHT-DOWN
  // Near face
  -0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f, // BOTTOM-LEFT-UP
   0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,// BOTTOM-RIGHT-UP
  -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,// BOTTOM-LEFT-DOWN
   0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,// BOTTOM-RIGHT-DOWN
  // Down face
  -0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f,// TOP-LEFT-DOWN
   0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f,// TOP-RIGHT-DOWN
  -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,// BOTTOM-LEFT-DOWN
   0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,// BOTTOM-RIGHT-DOWN
  };

  const std::vector<unsigned int> indices = {
    // Up face
    0,1,2,
    1,2,3,
    // Left face
    4,5,6,
    5,6,7,
    // Far face
    8,9,10,
    9,10,11,
    // Right face
    12,13,14,
    13,14,15,
    // Near face
    16,17,18,
    17,18,19,
    // Down face
    20,21,22,
    21,22,23,
  };
} // namespace withNormals

namespace withNormalsAndTexCoords {
// Position as a vec3 - normals as vec3 - Texture coordinates as vec2
const std::vector<float> vertices = {
    // Up face
    -0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 1.f,  // TOP-LEFT-UP
    0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f, 1.f,   // TOP-RIGHT-UP
    -0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, // BOTTOM-LEFT-UP
    0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f, 0.f,  // BOTTOM-RIGHT-UP
    // Left face
    -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f, 0.f, 1.f,  // TOP-LEFT-UP
    -0.5f, -0.5f, 0.5f, -1.f, 0.f, 0.f, 1.f, 1.f, // BOTTOM-LEFT-UP
    -0.5f, 0.5f, -0.5f, -1.f, 0.f, 0.f, 0.f, 0.f, // TOP-LEFT-DOWN
    -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 1.f, 0.f, // BOTTOM-LEFT-DOWN
    // Far face
    -0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f, // TOP-LEFT-UP
    0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 1.f, 1.f,  // TOP-RIGHT-UP
    -0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f,0.f, 0.f, // TOP-LEFT-DOWN
    0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f, 0.f, // TOP-RIGHT-DOWN
                                       // Right face
    0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f, 0.f, 1.f,  // TOP-RIGHT-UP
    0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f, 1.f, // BOTTOM-RIGHT-UP
    0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 0.f, 0.f, // TOP-RIGHT-DOWN
    0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f, 0.f, // BOTTOM-RIGHT-DOWN
    // Near face
    -0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f, 0.f, 1.f, // BOTTOM-LEFT-UP
    0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f, 1.f, 1.f,  // BOTTOM-RIGHT-UP
    -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.f, 0.f, // BOTTOM-LEFT-DOWN
    0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 1.f, 0.f, // BOTTOM-RIGHT-DOWN
    // Down face
    -0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 1.f, // TOP-LEFT-DOWN
    0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f, 1.f, 1.f,  // TOP-RIGHT-DOWN
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.f, // BOTTOM-LEFT-DOWN
    0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 1.f, 0.f, // BOTTOM-RIGHT-DOWN
};

const std::vector<unsigned int> indices = {
    // Up face
    0,
    1,
    2,
    1,
    2,
    3,
    // Left face
    4,
    5,
    6,
    5,
    6,
    7,
    // Far face
    8,
    9,
    10,
    9,
    10,
    11,
    // Right face
    12,
    13,
    14,
    13,
    14,
    15,
    // Near face
    16,
    17,
    18,
    17,
    18,
    19,
    // Down face
    20,
    21,
    22,
    21,
    22,
    23,
};
} // namespace withNormalsAndTexCoords
// clang-format on

} // namespace mesh::lighting
