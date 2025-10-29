#include <libs/core/Layer.hpp>

namespace libs::core {
Layer::Layer(const std::string &name) : m_debugName(name) {}

Layer::~Layer() {}

void Layer::onAttach() {}

void Layer::onDetach() {}

void Layer::onUpdate() {}

void Layer::onEvent(events::Event &event) {}

const std::string &Layer::getName() const { return m_debugName; }
} // namespace libs::core