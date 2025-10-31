#pragma once

#include <logl/events/export.h>

#include <libs/events/Event.hpp>

#include <format>

namespace libs::events {

class LOGL_EVENTS_EXPORT KeyEvent : public Event {
public:
  int getKeyCode() const { return m_keycode; }

protected:
  KeyEvent(int keycode) : m_keycode(keycode) {}
  int m_keycode;
};

class LOGL_EVENTS_EXPORT KeyPressedEvent : public KeyEvent {
public:
  // TODO: handle modifiers
  KeyPressedEvent(int keycode, bool isRepeat)
      : KeyEvent(keycode), m_isRepeat(isRepeat){};
  bool isRepeat() const { return m_isRepeat; }

  EVENT_CLASS_TYPE(KeyPressedEvent)

public:
  virtual const std::string toString() const override {
    return std::format("{}: keycode={} (repeat={})", getEventName(), m_keycode,
                       m_isRepeat);
    return getEventName();
  }

private:
  bool m_isRepeat;
};

class LOGL_EVENTS_EXPORT KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode){};
  EVENT_CLASS_TYPE(KeyReleasedEvent)

public:
  virtual const std::string toString() const override {
    return std::format("{}: keycode={}", getEventName(), m_keycode);
    return getEventName();
  }
};
} // namespace libs::events