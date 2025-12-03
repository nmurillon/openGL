#pragma once

#include <libs/events/Event.hpp>

#include <format>

namespace libs::events {
class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(double xPos, double yPos) : m_xPos(xPos), m_yPos(yPos) {}
  ~MouseMovedEvent() = default;
  double getXPos() const { return m_xPos; }
  double getYPos() const { return m_yPos; }
  virtual const std::string toString() const override {
    return std::format("{} (x={}, y={})", getEventName(), m_xPos, m_yPos);
  }

  EVENT_CLASS_TYPE(MouseMovedEvent)

private:
  double m_xPos;
  double m_yPos;
};

class MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(double xOffset, double yOffset)
      : m_xOffset(xOffset), m_yOffset(yOffset){};
  ~MouseScrolledEvent() = default;
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

class MouseButtonEvent : public Event {
public:
  MouseButtonEvent(int mouseButton) : m_mouseButton(mouseButton){};
  ~MouseButtonEvent() = default;
  double getMouseButton() const { return m_mouseButton; }

protected:
  int m_mouseButton;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(int mouseButton) : MouseButtonEvent(mouseButton){};
  ~MouseButtonPressedEvent() = default;

  virtual const std::string toString() const override {
    return std::format("{} : {}", getEventName(), m_mouseButton);
  }

  EVENT_CLASS_TYPE(MouseButtonPressedEvent)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(int mouseButton) : MouseButtonEvent(mouseButton){};
  ~MouseButtonReleasedEvent() = default;

  virtual const std::string toString() const override {
    return std::format("{} : {}", getEventName(), m_mouseButton);
  }

  EVENT_CLASS_TYPE(MouseButtonReleasedEvent)
};
} // namespace libs::events