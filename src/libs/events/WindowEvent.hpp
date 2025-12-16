#pragma once

#include <libs/events/Event.hpp>

#include <format>
#include <vector>

namespace libs::events {
class WindowCloseEvent : public Event {
public:
  WindowCloseEvent() = default;
  ~WindowCloseEvent() = default;

  EVENT_CLASS_TYPE(WindowCloseEvent)

public:
  virtual const std::string toString() const override { return getEventName(); }
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int width, int height) : m_width(width), m_height(height) {}
  ~WindowResizeEvent() = default;

  EVENT_CLASS_TYPE(WindowResizeEvent)

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

class PathDropEvent : public Event {
public:
  PathDropEvent(int count, const char **paths)
      : m_paths(paths, paths + count) {}
  ~PathDropEvent() = default;

  EVENT_CLASS_TYPE(PathDropEvent)

public:
  virtual const std::string toString() const override {
    return std::format("{} with {} paths\n", getEventName(), m_paths.size());
  }

  const std::vector<std::string> &getPaths() const { return m_paths; }

private:
  std::vector<std::string> m_paths;
};
} // namespace libs::events