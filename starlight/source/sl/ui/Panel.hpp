///
/// Panel.hpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PANEL_HPP_
#define STARLIGHT_PANEL_HPP_

#include <allegro5/allegro_color.h>

#include "sl/ui/Widget.hpp"

namespace sl
{
	///
	/// A UI Panel holds and positions UI widgets on it.
	///
	class Panel final
	{
	public:
		///
		/// Constructor.
		///
		/// \param bounds Dimension of the panel.
		/// \param colour Colour of the panel. Can be transparent or opaque.
		/// \param bgImage Optional background image to use.
		///
		explicit Panel(const sl::Rect<int>& bounds, const ALLEGRO_COLOR& colour, const std::string& bgImage = "");

		///
		/// Destructor.
		///
		~Panel();

		///
		/// Add a new widget to the Panel.
		///
		/// \param args Arguments for widget to construct.
		///
		/// \return Returns pointer to newly created widget.
		///
		template<typename _Widget, typename... Args>
		_Widget* addWidget(Args&&... args);

		///
		/// Update the UI.
		///
		void update();

		///
		/// Render Widgets.
		///
		void render();

		///
		/// Set visibility. Also affects widgets on panel.
		///
		void isVisible(bool isVisible);

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
		/// Is the panel currently visible. I.e. being rendered.
		///
		bool m_isVisible;

		///
		/// Dimensions of the panel, relative to the screen.
		///
		sl::Rect<int> m_bounds;

		///
		/// Background of the panel. Full colour or image.
		///
		ALLEGRO_BITMAP* m_background;

		///
		/// Collection of widgets on the panel.
		///
		std::vector<std::unique_ptr<Widget>> m_widgets;
	};

	template<typename _Widget, typename... Args>
	_Widget* Panel::addWidget(Args&&... args)
	{
		// Forward arguments to std::vector's construct in place method.
		_Widget* ref = m_widgets.emplace_back(std::make_unique<_Widget>(std::forward<Args>(args)...)).get();

		// Then return a pointer to object placed.
		return ref;
	}
}

#endif