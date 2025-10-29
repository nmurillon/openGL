#pragma once

#include <logl/core/export.h>

#include <libs/events/Event.hpp>

#include <string>

namespace libs::core {

class LOGL_CORE_EXPORT Layer {
public:
  Layer(const std::string &name = "Layer");
  virtual ~Layer();

  virtual void onAttach();
  virtual void onDetach();
  virtual void onUpdate();
  virtual void onEvent(events::Event &event);

  const std::string &getName() const;

protected:
  std::string m_debugName;
};
} // namespace libs::core