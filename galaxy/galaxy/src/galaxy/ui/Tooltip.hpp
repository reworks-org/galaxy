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
#include <qs/texture/NineSlice.hpp>

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
		/// \param ns_tex Must be a nine-slice texture on the UITheme texture atlas.
		/// \param text Text to draw on the tooltip.
		/// \param font Font to use.
		///
		void create(std::string_view ns_tex, const std::string& text, std::string_view font);

		///
		/// \brief Render the tooltip.
		///
		/// Required shaders: "text", "9slice".
		///
		/// \param camera View camera to use when rendering.
		///
		void render(qs::Camera& camera);

	private:
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

		///
		/// Nine slice info.
		///
		qs::NineSlice* m_ns;
	};

	///
	/// Shorthand.
	///
	using TooltipPtr = std::unique_ptr<galaxy::Tooltip>;
} // namespace galaxy

#endif