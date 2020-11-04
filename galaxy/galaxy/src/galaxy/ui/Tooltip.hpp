///
/// Tooltip.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOOLTIP_HPP_
#define GALAXY_TOOLTIP_HPP_

#include <qs/graphics/Camera.hpp>
#include <qs/sprite/Sprite.hpp>
#include <qs/text/Text.hpp>

#include "galaxy/ui/Theme.hpp"

///
/// Core namespace.
///
namespace galaxy
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
		/// Move constructor.
		///
		Tooltip(Tooltip&&) = default;

		///
		/// Copy assignment operator.
		///
		Tooltip& operator=(const Tooltip&) = delete;

		///
		/// Move assignment operator.
		///
		Tooltip& operator=(Tooltip&&) = default;

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
		/// Required shaders: "text", "sprite".
		///
		/// \param camera View camera to use when rendering.
		///
		void render(qs::Camera& camera);

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
		qs::Text m_text;

		///
		/// Texture Atlas Sprite.
		///
		qs::Sprite m_sprite;

		///
		/// UI theme for tooltip.
		///
		UITheme* m_theme;
	};

	///
	/// Shorthand.
	///
	using TooltipPtr = std::unique_ptr<galaxy::Tooltip>;
} // namespace galaxy

#endif