#include <libs/events/Event.hpp>

namespace libs::events {
bool Event::isHandled() const { return m_handled; }
} // namespace libs::events