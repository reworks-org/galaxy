///
/// Text.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXT_HPP_
#define QUASAR_TEXT_HPP_

#include "qs/text/Font.hpp"
#include "qs/core/Colours.hpp"
#include "qs/graphics/Sprite.hpp"
#include "qs/core/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec(s).
	///
	class Window;
	class Shader;
	class Renderer;

	///
	///
	///
	class Text final
	{
	public:
		///
		///
		///
		Text() noexcept;

		///
		///
		///
		explicit Text(const std::string& text, qs::Font& font, protostar::Colour& col) noexcept;

		///
		///
		///
		~Text();

		///
		/// Load resources used.
		///
		void load(const std::string& text, qs::Font& font, protostar::Colour& col);

		///
		///
		///
		void create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader);

		qs::Sprite& asSprite() noexcept;
		qs::RenderTexture& getTexture() noexcept;

	private:
		///
		///
		///
		std::string m_text;

		///
		///
		///
		qs::Font m_font;

		///
		///
		///
		protostar::Colour m_colour;

		///
		///
		///
		qs::RenderTexture m_texture;

		///
		///
		///
		qs::Sprite m_sprite;
	};
}

#endif