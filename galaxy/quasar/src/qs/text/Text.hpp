///
/// Text.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXT_HPP_
#define QUASAR_TEXT_HPP_

#include <protostar/system/Colour.hpp>

#include "qs/text/Font.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Creates text for use with OpenGL.
	///
	class Text final : public qs::VertexData
	{
	public:
		///
		/// Constructor.
		///
		Text() noexcept;

		///
		/// Default destructor.
		///
		~Text() noexcept = default;

		///
		/// Load resources used.
		///
		/// \param text Text to draw.
		/// \param font Font to apply to text. Holds a reference, does not store.
		/// \param col Colour of the text.
		///
		void load(const std::string& text, qs::Font* font, const protostar::Colour& col) noexcept;

		///
		/// Creates text from character verticies.
		///
		void create() noexcept;

		///
		/// \brief Update text.
		///
		/// Do not call load() or create(). Use this.
		/// Not thread safe. Calls GL code.
		/// If you need to have a string you want to update i.e. health: 20.
		/// Its better to split into 2 strings so health is static and the 20 is dynamic.
		/// Much faster.
		///
		/// \param text The new text.
		///
		void updateText(const std::string& text) noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		const float getOpacity() const noexcept;

		///
		/// Activate sprite context.
		///
		void bind() noexcept;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept;

	private:
		///
		/// Text.
		///
		std::string m_text;

		///
		/// Font.
		///
		qs::Font* m_font;

		///
		/// Colour.
		///
		protostar::Colour m_colour;

		///
		/// Opacity of sprite.
		///
		float m_opacity;
	};
}

#endif