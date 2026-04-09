#include "libs/io/ProgramPath.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <limits.h>
#include <mach-o/dyld.h>
#elif __linux__
#include <limits.h>
#include <unistd.h>
#endif

#include <stdexcept>

namespace libs::io {

ProgramPath &ProgramPath::getInstance() {
  static ProgramPath pp{};

  return pp;
}

ProgramPath::ProgramPath() {
#ifdef _WIN32
  // TODO: test this
  wchar_t path[FILENAME_MAX] = {0};
  GetModuleFileNameW(nullptr, path, FILENAME_MAX);
  m_programPath = std::filesystem::path(path);
#else
  char path[FILENAME_MAX] = {0};
  ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
  if (count < 0) {
    throw std::runtime_error("Error when retrieving program location");
  }

  m_programPath = std::filesystem::path(path);
#endif
}

const std::filesystem::path &ProgramPath::getProgramPath() const {
  return m_programPath;
}

const std::filesystem::path ProgramPath::getProgramDir() const {
  return m_programPath.parent_path();
}
} // namespace libs::io