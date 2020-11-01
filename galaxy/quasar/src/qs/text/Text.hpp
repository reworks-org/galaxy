///
/// Text.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXT_HPP_
#define QUASAR_TEXT_HPP_

#include <protostar/graphics/Colour.hpp>

#include "qs/core/Shader.hpp"
#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"
#include "qs/text/Font.hpp"
#include "qs/texture/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Creates text for use with OpenGL.
	///
	class Text final : public qs::VertexData, public qs::Transform
	{
	public:
		///
		/// Constructor.
		///
		Text();

		///
		/// Copy constructor.
		///
		Text(const Text&) = delete;

		///
		/// Move constructor.
		///
		Text(Text&&);

		///
		/// Copy assignment operator.
		///
		Text& operator=(const Text&) = delete;

		///
		/// Move assignment operator.
		///
		Text& operator=(Text&&);

		///
		/// Destructor.
		///
		~Text() = default;

		void load(Font& font, Shader& shader, const pr::Colour& col);
		void create(std::string_view text);
		void update_text(std::string_view text);

		///
		/// Activate context.
		///
		void bind();

		///
		/// Deactivate context.
		///
		void unbind();

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		[[nodiscard]] const int get_width() const;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		[[nodiscard]] const int get_height() const;

	private:
		///
		/// Colour of the text.
		///
		pr::Colour m_colour;

		///
		/// Pointer to font used for this text.
		///
		Font* m_font;

		///
		/// Pointer to shader used for rendering text characters.
		///
		Shader* m_shader;

		///
		/// Pre-rendered texture of text.
		///
		RenderTexture m_text;
	};
} // namespace qs

#endif