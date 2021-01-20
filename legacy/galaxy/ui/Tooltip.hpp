///
/// Tooltip.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_TOOLTIP_HPP_
#define GALAXY_UI_TOOLTIP_HPP_

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"
#include "galaxy/graphics/text/Text.hpp"
#include "galaxy/ui/Theme.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// Tooltip for widgets.
		///
		class Tooltip final
		{
			///
			/// Allows for the GUI to modify internals of Widget we don't want publicly exposed.
			///
			friend class GUI;

		public:
			///
			/// Constructor.
			///
			Tooltip();

			///
			/// Copy constructor.
			///
			Tooltip(const Tooltip&) = delete;

			///
			/// Copy assignment operator.
			///
			Tooltip& operator=(const Tooltip&) = delete;

			///
			/// Destructor.
			///
			~Tooltip() = default;

			///
			/// Create the tooltip.
			///
			/// \param sprite Sprite background of the tooltip.
			/// \param text Text to draw on the tooltip.
			/// \param font Font to use.
			///
			void create(std::string_view sprite, const std::string& text, std::string_view font);

			///
			/// \brief Render the tooltip.
			///
			/// Required shaders: "glyph", "sprite".
			///
			/// \param camera View camera to use when rendering.
			///
			void render(graphics::Camera& camera);

			///
			/// Update tooltip position.
			///
			/// \param x Update tooltip x pos.
			/// \param y Update tooltip y pos.
			///
			void update_pos(const double x, const double y);

			///
			/// Draw tooltip toggle.
			///
			/// \param can_draw True if he can draw.
			///
			void can_draw(const bool can_draw);

			///
			/// Can draw the tooltip.
			///
			/// \return Const boolean. True if tooltip is visible.
			///
			[[nodiscard]] const bool can_draw() const;

		private:
			///
			/// Draw tooltip toggle.
			///
			bool m_draw;

			///
			/// Text to display.
			///
			graphics::Text m_text;

			///
			/// Texture Atlas Sprite.
			///
			graphics::Sprite m_sprite;

			///
			/// UI theme for tooltip.
			///
			UITheme* m_theme;
		};
	} // namespace ui
} // namespace galaxy

#endif