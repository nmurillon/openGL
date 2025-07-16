#pragma once

#include <filesystem>

namespace libs::io {
class ProgramPath {
public:
  ProgramPath(const ProgramPath &) = delete;
  ProgramPath(const ProgramPath &&) = delete;
  ProgramPath &operator=(const ProgramPath &) = delete;
  ProgramPath &operator=(const ProgramPath &&) = delete;

  static ProgramPath &getInstance();
  const std::filesystem::path &getProgramPath() const;
  const std::filesystem::path getProgramDir() const;

private:
  ProgramPath();
  std::filesystem::path m_programPath;
};
} // namespace libs::io