#include <logl/events/export.h>

#include <libs/events/Event.hpp>

#include <sstream>

namespace libs::events {
class LOGL_EVENTS_EXPORT WindowCloseEvent : public Event {
public:
  WindowCloseEvent()
      : Event(EventType::WindowCloseEvent, "Window close event") {}
};

class LOGL_EVENTS_EXPORT WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int width, int height)
      : Event(EventType::WindowResizeEvent, "Window resize event"),
        m_width(width), m_height(height) {}

  ~WindowResizeEvent() = default;

  virtual std::string toString() const override {
    std::stringstream ss;
    ss << getEventName() << std::endl
       << "New size: (width: " << m_width << ", height: " << m_height << ")"
       << std::endl;

    return ss.str();
  }

  int getWidth() const { return m_width; }

  int getHeight() const { return m_height; }

private:
  int m_width;
  int m_height;
};
} // namespace libs::events