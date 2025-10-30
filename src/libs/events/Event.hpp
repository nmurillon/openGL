#pragma once

#include <logl/events/export.h>

#include <ostream>
#include <string>
namespace libs::events {
enum class LOGL_EVENTS_EXPORT EventType {
  None,
  // Window Events
  WindowCloseEvent,
  WindowResizeEvent, // TODO: add new events as needed

  // Key events
  KeyPressedEvent,
  KeyReleasedEvent,

  // Mouse events
  MouseButtonPressedEvent,
  MouseButtonReleasedEvent,
  MouseMouvedEvent,
  MouseScrolledEvent
};

class LOGL_EVENTS_EXPORT Event {
public:
  Event(const EventType &type, const std::string &name);
  virtual ~Event() = default;

  bool isHandled() const;
  bool handle();
  const EventType &getEventType() const;
  const std::string &getEventName() const;
  virtual std::string toString() const;

protected:
  bool m_handled{false};
  EventType m_type;

  // Debug info
  std::string m_name;
};
} // namespace libs::events

std::ostream &operator<<(std::ostream &o, const libs::events::Event &event);