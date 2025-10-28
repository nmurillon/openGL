#pragma once

#include <logl/core/export.h>

#include <libs/core/Window.hpp>

#include <memory>
#include <string>

namespace libs::core {
class LOGL_CORE_EXPORT Application {
public:
  Application(const std::string &name);
  Application(const Application &) = delete;
  Application(Application &&) = delete;
  Application operator=(const Application &) = delete;
  Application operator=(Application &&) = delete;
  virtual ~Application();

  void run();

protected:
  [[nodiscard]] Window &getWindow();
  [[nodiscard]] const Window &getWindow() const;

private:
  std::unique_ptr<Window> m_mainWindow;
};
} // namespace libs::core