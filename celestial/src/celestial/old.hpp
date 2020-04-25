///
/// UI.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UI_HPP_
#define CELESTIAL_UI_HPP_

#include <vector>

#include <protostar/async/ThreadPool.hpp>

#include "celestial/Widget.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Manages UI for a dynamic array of Panels.
	/// Stores panels as unique_ptrs.
	///
	class UI final
	{
	public:
		///
		/// Constructor.
		///
		UI() noexcept;

		///
		/// Destructor.
		///
		~UI() noexcept;

		///
		/// Update the UI.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) noexcept;

		///
		/// Draw UI to screen.
		///
		void render() noexcept;

		///
		/// Add a new widget to the Panel.
		///
		/// \param args Arguments for widget to construct.
		///
		/// \return Returns pointer to newly created widget.
		///
		template<typename WidgetType, typename... Args>
		WidgetType* add(Args&&... args) noexcept;

		///
		/// Remove a widget.
		///
		/// \param id ID of widget to remove. Get by calling widget->id().
		///
		void remove(const unsigned int id) noexcept;

		///
		/// Destroy all widgets.
		///
		void destroy() noexcept;

		///
		/// Set visibility of UI, and it's widgets.
		///
		/// \param isVisible True if visible.
		///
		void isVisible(const bool isVisible) noexcept;

		///
		/// Set renderer.
		///
		/// \param renderer Renderer to use when drawing.
		///
		void setRenderer(celestial::interface::Renderer* renderer) noexcept;

		///
		/// Set UI theme.
		///
		/// \param theme UI theme to use.
		///
		void setTheme(celestial::UITheme* theme) noexcept;

	private:
		///
		/// UI Theme.
		///
		celestial::UITheme* m_theme;

		///
		/// Renderer to use when drawing.
		///
		celestial::interface::Renderer* m_renderer;

		///
		/// Collection of widgets on the panel.
		///
		std::vector<celestial::WidgetPtr> m_widgets;

		///
		/// Is the UI visible.
		///
		bool m_isVisible;

		///
		/// ID Counter.
		///
		unsigned int m_counter;

		///
		/// List of free'd up IDs.
		///
		std::vector<unsigned int> m_free;

		///
		/// Thead Pool.
		///
		protostar::ThreadPool m_pool;
	};

	template<typename WidgetType, typename... Args>
	inline WidgetType* UI::add(Args&&... args) noexcept
	{
		unsigned int idToUse = 0;

		if (m_free.size() > 0)
		{
			idToUse = m_free[0];

			m_free.erase(m_free.begin());
			m_free.shrink_to_fit();
		}
		else
		{
			idToUse = m_counter;

			// Increment counter for next widget.
			++m_counter;
		}

		// Make sure widget is the right size.
		if (m_widgets.size() >= idToUse)
		{
			m_widgets.resize(idToUse);
		}

		// Forward arguments to std::vector's construct in place method.
		m_widgets.emplace(idToUse, std::make_unique<WidgetType>(std::forward<Args>(args)...));
		
		// Set ID.
		Widget* ref = m_widgets[idToUse].get();
		ref->m_id = idToUse;

		// Then return a pointer to object placed.
		return dynamic_cast<WidgetType*>(ref);
	}
}

#endif