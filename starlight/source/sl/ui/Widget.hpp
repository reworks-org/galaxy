///
/// Widget.hpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_WIDGET_HPP_
#define STARLIGHT_WIDGET_HPP_

#include "sl/math/Rect.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

namespace sl
{
	///
	/// Allows for widgets to be stored uniformly in a single data structure, because sl::Widget is a template class.
	///
	class BaseWidget
	{
	public:
		///
		/// \brief Render the Widget.
		///
		/// This should only contain code on rendering the widget. Remember to check for visibility.
		///
		virtual void render() = 0;

		///
		/// Set visibility.
		///
		void isVisible(bool isVisible);

	protected:
		///
		/// Protected constructor. Only want derived classes to construct this.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		explicit BaseWidget(const sl::Rect<int>& bounds) noexcept;

	public:
		///
		/// Dimensional bounds of the widget.
		///
		sl::Rect<int> m_bounds;

	private:
		///
		/// Is the panel currently visible. I.e. being rendered.
		///
		bool m_isVisible;
	};

	///
	/// \brief Represents an interactable UI object i.e. a button.
	///
	/// The template Event is the type of event the widget could be subscribed to.
	///
	template<typename Event>
	class Widget : public BaseWidget
	{
	public:
		///
		/// \brief Subscribe to an event.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		virtual void receive(const Event& e) = 0;

	protected:
		///
		/// Protected constructor. Only want derived classes to construct this.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		explicit Widget(const sl::Rect<int>& bounds) noexcept;
	};

	template<typename Event>
	Widget<Event>::Widget(const sl::Rect<int>& bounds) noexcept
		:BaseWidget(bounds)
	{
		// Adds recieve() method to dispatcher automatically.

	}
}

#endif