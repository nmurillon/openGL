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

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType getStaticType() { return EventType::type; }                 \
  virtual EventType getEventType() const override { return getStaticType(); }  \
  virtual const std::string getEventName() const override { return m_name; }   \
                                                                               \
protected:                                                                     \
  std::string m_name{#type};

class LOGL_EVENTS_EXPORT Event {
public:
  Event() = default;
  virtual ~Event() = default;

  bool isHandled() const;
  bool handle();
  static EventType GetStaticType() { return EventType::None; }
  virtual EventType getEventType() const = 0;
  virtual const std::string getEventName() const = 0;
  virtual const std::string toString() const = 0;

protected:
  bool m_handled{false};

  // Debug info
  std::string m_name{"Basic Event"};
};
} // namespace libs::events

std::ostream &operator<<(std::ostream &o, const libs::events::Event &event);