///
/// Panel.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_PANEL_HPP_
#define CELESTIAL_PANEL_HPP_

#include "celestial/ui/Widget.hpp"
#include "celestial/ui/UITheme.hpp"
#include "protostar/graphics/Rect.hpp"
#include "celestial/interface/Renderer.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// A UI Panel holds and positions UI widgets on it.
	///
	class Panel final
	{
	public:
		///
		/// Primitives Constructor.
		///
		/// \param bounds Dimension of the panel.
		/// \param theme Theme of the panel UI.
		///
		Panel(const protostar::Rect<int>& bounds, UITheme* theme);

		///
		/// Image Constructor.
		///
		/// \param texture Texture from the theme's master texture to use.
		/// \param bounds Dimension of the panel.
		/// \param theme Theme to use containing panel texture.
		///
		Panel(const std::string& texture, const protostar::Rect<int>& bounds, UITheme* theme);

		///
		/// Destructor.
		///
		~Panel() noexcept;

		///
		/// Add a new widget to the Panel.
		///
		/// \param args Arguments for widget to construct.
		///
		/// \return Returns pointer to newly created widget.
		///
		template<typename WidgetType, typename... Args>
		WidgetType* add(Args&&... args);

		///
		/// Remove a widget.
		///
		/// \param id ID of widget to remove. Get by calling widget->id().
		///
		void remove(unsigned int id);

		///
		/// Update the panel and widgets.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt);

		///
		/// Render panel and widgets.
		///
		/// \param renderer Renderer to use when drawing. Needs to be consistent.
		///
		void render(celestial::compat::Renderer* renderer);

		///
		/// Set visibility of panel and subsiquently all widgets.
		///
		/// \param isVisible True if visible.
		///
		void setVisibility(const bool isVisible);

		///
		/// Destroy all widgets.
		///
		void clear();

	private:
		///
		/// Primary Constructor.
		///
		Panel() = delete;

	private:
		///
		/// ID Counter;
		///
		unsigned int m_counter;

		///
		/// Is the panel currently visible. I.e. being rendered.
		///
		bool m_isVisible;

		///
		/// Dimensions of the panel, relative to the screen.
		///
		protostar::Rect<int> m_bounds;

		///
		/// Background of the panel. Full colour or texture.
		///
		TexturePtr m_background;

		///
		/// Collection of widgets on the panel.
		///
		std::unordered_map<unsigned int, std::unique_ptr<Widget>> m_widgets;

		///
		/// UITheme used by the panel.
		///
		UITheme* m_theme;
	};

	template<typename WidgetType, typename... Args>
	inline WidgetType* Panel::add(Args&&... args)
	{
		// Forward arguments to std::vector's construct in place method.
		m_widgets.emplace(m_counter, std::make_unique<WidgetType>(std::forward<Args>(args)...));
		Widget* ref = m_widgets[m_counter].get();
		
		// Set offset and id.
		ref->setOffset(m_bounds.m_x, m_bounds.m_y);
		ref->m_id = m_counter;

		// Increment counter for next widget.
		++m_counter;

		// Then return a pointer to object placed.
		return dynamic_cast<WidgetType*>(ref);
	}
}

#endif