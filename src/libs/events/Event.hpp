#pragma once

#include <ostream>
#include <string>

namespace libs::events {
enum class EventType {
  None,
  // Window Events
  WindowCloseEvent,
  WindowResizeEvent, // TODO: add new events as needed
  PathDropEvent,

  // Key events
  KeyPressedEvent,
  KeyReleasedEvent,

  // Mouse events
  MouseButtonPressedEvent,
  MouseButtonReleasedEvent,
  MouseMovedEvent,
  MouseScrolledEvent
};

#define EVENT_CLASS_TYPE(type)                                                 \
public:                                                                        \
  static EventType getStaticType() { return EventType::type; }                 \
  virtual EventType getEventType() const override { return getStaticType(); }  \
  virtual const std::string getEventName() const override { return m_name; }   \
                                                                               \
protected:                                                                     \
  std::string m_name{#type};

class Event {
public:
  Event() = default;
  virtual ~Event() = default;

  bool isHandled() const { return m_handled; }
  bool handle() {
    m_handled = true;

    return m_handled;
  }

  static EventType getStaticType() { return EventType::None; }
  virtual EventType getEventType() const { return getStaticType(); }
  virtual const std::string getEventName() const { return m_name; }
  virtual const std::string toString() const = 0;

protected:
  bool m_handled{false};

  // Debug info
  std::string m_name{"Basic Event"};
};

inline std::ostream &operator<<(std::ostream &o,
                                const libs::events::Event &event) {
  o << event.toString();

  return o;
}

} // namespace libs::events