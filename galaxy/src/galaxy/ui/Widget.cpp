/*
///
/// Widget.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/SoundBook.hpp"

#include "Widget.hpp"

namespace galaxy
{
namespace ui
{
const unsigned int Widget::id() const noexcept
{
return m_id;
}

Widget::Widget(const Widget::Type type) noexcept
: Serializable {this}, m_id {0}, m_theme {nullptr}, m_tooltip {nullptr}, m_bounds {0.0f, 0.0f, 0.0f, 0.0f}, m_type {type}
{
}

Widget::Widget(Widget&& w) noexcept
: Serializable {this}
{
this->m_id      = w.m_id;
this->m_theme   = w.m_theme;
this->m_tooltip = std::move(w.m_tooltip);

w.m_id = 0;
}

Widget& Widget::operator=(Widget&& w) noexcept
{
if (this != &w)
{
this->m_id      = w.m_id;
this->m_theme   = w.m_theme;
this->m_tooltip = std::move(w.m_tooltip);

w.m_id = 0;
}

return *this;
}

Widget::~Widget()
{
m_tooltip.reset();
m_theme = nullptr;
}
} // namespace ui
} // namespace galaxy
*/