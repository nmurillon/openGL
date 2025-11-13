#include <libs/renderer/Shader.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace libs::renderer {
Shader::Shader(const std::string &vertexSrcFile,
               const std::string &fragmentSrcFile) {

  const unsigned int vertexShader =
      readShaderFile(vertexSrcFile, GL_VERTEX_SHADER);
  const unsigned int fragmentShader =
      readShaderFile(fragmentSrcFile, GL_FRAGMENT_SHADER);

  m_shaderId = glCreateProgram();

  // Beware to the order
  glAttachShader(m_shaderId, vertexShader);
  glAttachShader(m_shaderId, fragmentShader);

  glLinkProgram(m_shaderId);

  // Debug comile errors if any
  char error[512];
  int success;

  glGetProgramiv(m_shaderId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(m_shaderId, 512, NULL, error);
    std::cout << std::format("ERROR::SHADER::LINKING_FAILED\n{}", error)
              << std::endl;
  };

  // Once linked, shaders can be deleted
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() const { glUseProgram(m_shaderId); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(getLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(getLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(getLocation(name), value);
}

void Shader::setVec2f(const std::string &name, const glm::vec2 &value) const {
  glUniform2f(getLocation(name), value.x, value.y);
}

void Shader::setVec2f(const std::string &name,
                      const std::vector<float> &value) const {
  if (2 != value.size()) {
    throw std::runtime_error(std::format(
        "Can not set values for uniform. Expecting 2, but {} were provided",
        value.size()));
  }
  glUniform2f(getLocation(name), value.at(0), value.at(1));
}

void Shader::setVec2f(const std::string &name, float x, float y) const {
  glUniform2f(getLocation(name), x, y);
}

void Shader::setVec3f(const std::string &name, const glm::vec3 &value) const {
  glUniform3f(getLocation(name), value.x, value.y, value.z);
}

void Shader::setVec3f(const std::string &name,
                      const std::vector<float> &value) const {
  if (3 != value.size()) {
    throw std::runtime_error(std::format(
        "Can not set values for uniform. Expecting 3, but {} were provided",
        value.size()));
  }
  glUniform3f(getLocation(name), value.at(0), value.at(1), value.at(2));
}

void Shader::setVec3f(const std::string &name, float x, float y,
                      float z) const {
  glUniform3f(getLocation(name), x, y, z);
}

void Shader::setVec4f(const std::string &name, const glm::vec4 &value) const {

  glUniform4f(getLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setVec4f(const std::string &name,
                      const std::vector<float> &value) const {
  if (4 != value.size()) {
    throw std::runtime_error(std::format(
        "Can not set values for uniform. Expecting 4, but {} were provided",
        value.size()));
  }
  glUniform4f(getLocation(name), value.at(0), value.at(1), value.at(2),
              value.at(3));
}

void Shader::setMat4f(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::readShaderFile(const std::string &src, unsigned int type) {
  std::ifstream srcStream{src};
  if (!srcStream.is_open() || !srcStream.good()) {
    throw std::runtime_error(
        std::format("Failed to open vertex shader file: {}", src));
  }

  std::stringstream srcSS;
  srcSS << srcStream.rdbuf();

  const std::string srcString = srcSS.str();
  const char *content = srcString.c_str();

  const unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &content, NULL);
  glCompileShader(shader);

  // Debug compile errors if any
  char error[512];
  int success;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, error);
    std::cout << std::format("ERROR::SHADER::COMPILATION_FAILED\n"
                             "Source file: {}\n"
                             "{}",
                             src, error)
              << std::endl;
  };

  return shader;
}

GLint Shader::getLocation(const std::string &name) const {
  return glGetUniformLocation(m_shaderId, name.c_str());
}
} // namespace libs::renderer