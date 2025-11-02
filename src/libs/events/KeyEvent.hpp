#pragma once

#include <libs/events/Event.hpp>

#include <format>

namespace libs::events {

class KeyEvent : public Event {
public:
  KeyEvent(int keycode) : m_keycode(keycode) {}
  ~KeyEvent() = default;
  int getKeyCode() const { return m_keycode; }

protected:
  int m_keycode;
};

class KeyPressedEvent : public KeyEvent {
public:
  // TODO: handle modifiers
  KeyPressedEvent(int keycode, bool isRepeat)
      : KeyEvent(keycode), m_isRepeat(isRepeat){};
  ~KeyPressedEvent() = default;
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

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(int keycode) : KeyEvent(keycode){};
  ~KeyReleasedEvent() = default;
  EVENT_CLASS_TYPE(KeyReleasedEvent)

public:
  virtual const std::string toString() const override {
    return std::format("{}: keycode={}", getEventName(), m_keycode);
    return getEventName();
  }
};
} // namespace libs::events