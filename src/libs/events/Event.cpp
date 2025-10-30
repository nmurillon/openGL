#include <libs/events/Event.hpp>

namespace libs::events {

bool Event::isHandled() const { return m_handled; }

bool Event::handle() {
  m_handled = true;

  return m_handled;
}
} // namespace libs::events

std::ostream &operator<<(std::ostream &o, const libs::events::Event &event) {
  o << event.toString();

  return o;
}