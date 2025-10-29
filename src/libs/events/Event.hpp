#pragma once

#include <logl/events/export.h>

namespace libs::events {
enum class LOGL_EVENTS_EXPORT EventType { None };

class LOGL_EVENTS_EXPORT Event {
public:
  bool isHandled() const;

protected:
  bool m_handled{false};
};
} // namespace libs::events