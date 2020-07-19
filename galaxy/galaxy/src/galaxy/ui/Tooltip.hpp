///
/// Tooltip.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TOOLTIP_HPP_
#define GALAXY_TOOLTIP_HPP_

#include <qs/text/Text.hpp>
#include <qs/sprite/BatchedSprite.hpp>
#include <qs/graphics/Camera.hpp>
#include <protostar/system/Colour.hpp>
#include <protostar/events/MouseMovedEvent.hpp>

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
	public:
		///
		/// Argument constructor.
		///
		/// \param theme Pointer to theme object.
		///
		Tooltip(Theme* theme) noexcept;

		///
		/// Destructor.
		///
		~Tooltip() noexcept;

		///
		/// Allows for buttons to react to mouse events.
		///
		/// \param e A MouseMovedEvent object to check if tooltip is active.
		///
		void onMove(const pr::MouseMovedEvent& e) noexcept;

		///
		/// Render the tooltip.
		///
		/// \param camera Camera position to utilize when rendering.
		/// \param textShader ID of shader in ShaderBook.
		///
		void render(qs::Camera& camera, std::string_view textShader) noexcept;
		
		///
		/// Set the text of the tooltip.
		///
		/// \param text Text of the tooltip.
		///
		void setText(const std::string& text) noexcept;

		///
		/// Set colour of text.
		///
		/// \param colour Colour of the text to use.
		///
		void setColour(const pr::Colour& colour) noexcept;

		///
		/// Set the background texture of the tooltip.
		///
		/// \param texture ID of texture in textureatlas to use.
		///
		void setTexture(const std::string& texture) noexcept;

		///
		/// Create tooltip from provided data.
		///
		/// \param font Font ID in the resource cache.
		///
		void create(std::string_view font) noexcept;

		///
		/// Update text.
		///
		void updateText(const std::string& update) noexcept;

	private:
		///
		/// Default constructor.
		///
		Tooltip() noexcept = delete;

	private:
		///
		/// X position.
		///
		float m_x;

		///
		/// Y position.
		///
		float m_y;

		///
		/// Text of tooltip.
		///
		std::string m_textStr;

		///
		/// Colour of text.
		///
		pr::Colour m_colour;

		///
		/// Texture string.
		///
		std::string m_texture;

		///
		/// Theme of the tooltip.
		///
		Theme* m_theme;

		///
		/// Text to draw.
		///
		qs::Text m_text;

		///
		/// Background to draw.
		///
		qs::BatchedSprite m_sprite;
	};
}

#endif