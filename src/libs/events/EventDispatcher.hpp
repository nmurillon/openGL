#pragma once

#include <logl/events/export.h>

#include <libs/events/Event.hpp>

#include <functional>

#define LOGL_BIND_EVENT_FN(fn)                                                 \
  [this](auto &&...args) -> decltype(auto) {                                   \
    return this->fn(std::forward<decltype(args)>(args)...);                    \
  }

namespace libs::events {
class LOGL_EVENTS_EXPORT EventDispatcher {
public:
  EventDispatcher(Event &event) : m_event(event){};
  EventDispatcher() = delete;

  template <typename T> void dispatch(std::function<void(T &)> func) {
    if (m_event.getEventType() == T::getStaticType()) {
      func(static_cast<T &>(m_event));
    }
  }

private:
  Event &m_event;
};
} // namespace libs::events