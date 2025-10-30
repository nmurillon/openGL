#include <logl/events/export.h>

#include <libs/events/Event.hpp>

#include <format>

namespace libs::events {
class LOGL_EVENTS_EXPORT WindowCloseEvent : public Event {
public:
  WindowCloseEvent() = default;

  EVENT_CLASS_TYPE(WindowCloseEvent);

public:
  virtual const std::string toString() const override { return getEventName(); }
};

class LOGL_EVENTS_EXPORT WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}

  ~WindowResizeEvent() = default;

  EVENT_CLASS_TYPE(WindowResizeEvent);

public:
  virtual const std::string toString() const override {
    return std::format("{} with new size (w={}, h={})\n", getEventName(),
                       m_width, m_height);
  }

  int getWidth() const { return m_width; }

  int getHeight() const { return m_height; }

private:
  int m_width;
  int m_height;
};
} // namespace libs::events