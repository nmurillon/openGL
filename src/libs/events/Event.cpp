#include <libs/events/Event.hpp>

namespace libs::events {
Event::Event(const EventType &type, const std::string &name)
    : m_type(type), m_name(name) {}

bool Event::isHandled() const { return m_handled; }

bool Event::handle() {
  m_handled = true;

  return m_handled;
}

const EventType &Event::getEventType() const { return m_type; }

const std::string &Event::getEventName() const { return m_name; }

std::string Event::toString() const { return getEventName(); }
} // namespace libs::events

std::ostream &operator<<(std::ostream &o, const libs::events::Event &event) {
  o << event.toString();

  return o;
}