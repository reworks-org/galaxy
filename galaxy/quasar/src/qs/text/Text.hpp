///
/// Text.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXT_HPP_
#define QUASAR_TEXT_HPP_

#include "qs/text/Font.hpp"
#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

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
		Text() noexcept;

		///
		/// Copy constructor.
		///
		Text(const Text&) noexcept = delete;

		///
		/// Move constructor.
		///
		Text(Text&&);

		///
		/// Copy assignment operator.
		///
		Text& operator=(const Text&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Text& operator=(Text&&);

		///
		/// Destructor.
		///
		virtual ~Text() noexcept = default;

		///
		/// Load resources used.
		///
		/// \param text Text to draw.
		/// \param font Font to apply to text. Cannot be nullptr.
		/// \param col Colour of the text.
		///
		void load(const std::string& text, qs::Font* font, const pr::Colour& col) noexcept;

		///
		/// Creates text from character verticies.
		///
		void create();

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
		void update_text(const std::string& text);

		///
		/// Activate sprite context.
		///
		void bind() noexcept;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept;

		///
		/// Get current text width.
		///
		const float get_width() const noexcept;

		///
		/// Get current text height.
		///
		const float get_height() const noexcept;

	private:
		///
		/// Current width.
		///
		float m_width;

		///
		/// Current height.
		///
		float m_height;

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
		pr::Colour m_colour;

		///
		/// Stores indexs.
		///
		std::vector<unsigned int> m_is;

		///
		/// Stores vertexs.
		///
		std::vector<qs::SpriteVertex> m_vs;
	};
} // namespace qs

#endif