#include <libs/renderer/Shader.hpp>

#include <libs/io/ProgramPath.hpp>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

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
    std::cout << "ERROR::SHADER::LINKING_FAILED\n" << error << std::endl;
  };

  // Once linked, shaders can be deleted
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() const { glUseProgram(m_shaderId); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()),
              static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setVec3f(const std::string &name, std::vector<float> value) const {
  if (3 != value.size()) {
    throw std::runtime_error(
        "Can not set values for uniform. Expecting 3, but " +
        std::to_string(value.size()) + " were provided");
  }
  glUniform3f(glGetUniformLocation(m_shaderId, name.c_str()), value.at(0),
              value.at(1), value.at(2));
}

void Shader::setVec3f(const std::string &name, float x, float y,
                      float z) const {
  glUniform3f(glGetUniformLocation(m_shaderId, name.c_str()), x, y, z);
}

void Shader::setVec4f(const std::string &name, std::vector<float> value) const {
  if (4 != value.size()) {
    throw std::runtime_error(
        "Can not set values for uniform. Expecting 4, but " +
        std::to_string(value.size()) + " were provided");
  }
  glUniform4f(glGetUniformLocation(m_shaderId, name.c_str()), value.at(0),
              value.at(1), value.at(2), value.at(3));
}

void Shader::setMat4f(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_shaderId, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::readShaderFile(const std::string &src, unsigned int type) {
  const std::string shaderPath{
      (libs::io::ProgramPath::getInstance().getProgramDir() /
       LOGL_RENDERER_RESOURCES_FOLDER / src)
          .string()};

  std::ifstream srcStream{shaderPath};
  if (!srcStream.is_open() || !srcStream.good()) {
    throw std::runtime_error(
        std::string("Failed to open vertex shader file: ") + shaderPath);
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
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
              << "Source file: " << src << "\n"
              << error << std::endl;
  };

  return shader;
}
} // namespace libs::renderer