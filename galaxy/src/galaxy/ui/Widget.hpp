/*
///
/// Widget.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGET_HPP_
#define GALAXY_UI_WIDGET_HPP_

#include "galaxy/components/Transform2D.hpp"
#include "galaxy/ui/Tooltip.hpp"

namespace galaxy
{
namespace ui
{
///
/// \brief Interactable UI object.
///
/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
///
class Widget : public fs::Serializable
{
friend class GUI;

public:
///
/// The type of the widget.
///
enum class Type : short
{
BUTTON,
IMAGE,
LABEL,
PROGRESSBAR,
SLIDER,
TEXTBOX,
TEXTINPUT,
TOGGLEBUTTON
};

///
/// Move constructor.
///
Widget(Widget&&) noexcept;

///
/// Move assignment operator.
///
Widget& operator=(Widget&&) noexcept;

///
/// Destructor.
///
virtual ~Widget();

///
/// \brief Update widget logic.
///
/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
///
/// \param dt Delta Time from gameloop.
///
virtual void update(const double dt) = 0;

///
/// \brief Render the widget.
///
/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
///
virtual void render() = 0;

///
/// Create a tooltip and assign it to a widget.
///
/// \param args Arguments to construct tooltip with.
///
template<typename... Args>
[[nodiscard]] Tooltip* create_tooltip(Args&&... args);

///
/// \brief Get ID of this widget.
///
/// 0 is reserved for NULL.
///
/// \return const unsigned int ID value.
///
[[nodiscard]] const unsigned int id() const noexcept;

protected:
///
/// Constructor.
///
/// \param type The Type of this widget.
///
Widget(const Widget::Type type) noexcept;

private:
///
/// Deleted default constructor.
///
Widget() = delete;

///
/// Copy constructor.
///
Widget(const Widget&) = delete;

///
/// Copy assignment operator.
///
Widget& operator=(const Widget&) = delete;

protected:
///
/// Pointer to GUI theme.
///
Theme* m_theme;

///
/// Tooltip. Optional.
///
std::unique_ptr<Tooltip> m_tooltip;

///
/// Bounds of widget.
///
math::Rect<float> m_bounds;

private:
///
/// Widget ID.
///
unsigned int m_id;

///
/// Widget type.
///
Widget::Type m_type;
};

template<typename... Args>
inline Tooltip* Widget::create_tooltip(Args&&... args)
{
Tooltip* ptr = nullptr;

if (m_tooltip != nullptr)
{
GALAXY_LOG(GALAXY_ERROR, "Attempted to add duplicate tooltip.");
}
else
{
m_tooltip    = std::make_unique<Tooltip>(std::forward<Args>(args)...);
ptr          = m_tooltip.get();
ptr->m_theme = m_theme;
}

return ptr;
}

///
/// Shorthand.
///
using WidgetPtr = std::unique_ptr<Widget>;
} // namespace ui
} // namespace galaxy

#endif
*/