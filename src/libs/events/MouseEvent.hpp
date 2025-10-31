#pragma once

#include <logl/events/export.h>

#include <libs/events/Event.hpp>

#include <format>

namespace libs::events {
class LOGL_EVENTS_EXPORT MouseMouvedEvent : public Event {
public:
  MouseMouvedEvent(double xPos, double yPos) : m_xPos(xPos), m_yPos(yPos){};
  double getXPos() const { return m_xPos; }
  double getYPos() const { return m_yPos; }
  virtual const std::string toString() const override {
    return std::format("{} (x={}, y={})", getEventName(), m_xPos, m_yPos);
  }

  EVENT_CLASS_TYPE(MouseMouvedEvent)

private:
  double m_xPos;
  double m_yPos;
};

class LOGL_EVENTS_EXPORT MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(double xOffset, double yOffset)
      : m_xOffset(xOffset), m_yOffset(yOffset){};
  double getXOffset() const { return m_xOffset; }
  double getYOffset() const { return m_yOffset; }
  virtual const std::string toString() const override {
    return std::format("{} (xoffset={}, yoffset={})", getEventName(), m_xOffset,
                       m_yOffset);
  }

  EVENT_CLASS_TYPE(MouseScrolledEvent)

private:
  double m_xOffset;
  double m_yOffset;
};

class LOGL_EVENTS_EXPORT MouseButtonEvent : public Event {
public:
  MouseButtonEvent(int mouseButton) : m_mouseButton(mouseButton){};
  double getMouseButton() const { return m_mouseButton; }

protected:
  int m_mouseButton;
};

class LOGL_EVENTS_EXPORT MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(int mouseButton) : MouseButtonEvent(mouseButton){};

  virtual const std::string toString() const override {
    return std::format("{} : {}", getEventName(), m_mouseButton);
  }

  EVENT_CLASS_TYPE(MouseButtonPressedEvent)
};

class LOGL_EVENTS_EXPORT MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(int mouseButton) : MouseButtonEvent(mouseButton){};

  virtual const std::string toString() const override {
    return std::format("{} : {}", getEventName(), m_mouseButton);
  }

  EVENT_CLASS_TYPE(MouseButtonReleasedEvent)
};
} // namespace libs::events