#pragma once

#include <logl/core/export.h>

#include <libs/core/LayerStack.hpp>
#include <libs/core/Window.hpp>
#include <libs/events/Event.hpp>

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

  void onEvent(events::Event &event);
  void addLayer(const std::shared_ptr<Layer> &layer);
  void addOverlayLayer(const std::shared_ptr<Layer> &layer);

protected:
  [[nodiscard]] Window &getWindow();
  [[nodiscard]] const Window &getWindow() const;

private:
  std::unique_ptr<Window> m_mainWindow;
  LayerStack m_layerStack;
};
} // namespace libs::core