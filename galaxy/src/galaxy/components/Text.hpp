///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TEXT_HPP_
#define GALAXY_COMPONENTS_TEXT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Font.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace sc
{
	namespace panel
	{
		class EntityEditor;
	} // namespace panel
} // namespace sc

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
	} // namespace systems

	namespace components
	{
		///
		/// String of glyphs rendered with a font.
		///
		class Text final : public graphics::Renderable, public fs::Serializable
		{
			friend class sc::panel::EntityEditor;
			friend class systems::RenderSystem;

		public:
			///
			/// Text alignment.
			///
			enum class Alignment : int
			{
				LEFT   = 0,
				CENTER = 1,
				RIGHT  = 2
			};

			///
			/// Constructor.
			///
			Text();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Text(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			Text(Text* ptr);

			///
			/// Move constructor.
			///
			Text(Text&&);

			///
			/// Move assignment operator.
			///
			Text& operator=(Text&&);

			///
			/// Destructor.
			///
			virtual ~Text();

			///
			/// Creates the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param font Font to use.
			/// \param colour Font colour.
			/// \param layer Rendering layer.
			/// \param alignment Text alignment. Defaults to LEFT.
			///
			void create(std::string_view text,
				const float size,
				const std::string& font,
				const graphics::Colour& colour,
				const int layer,
				Alignment alignment = Alignment::LEFT);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			///
			void update(std::string_view text);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			///
			void update(std::string_view text, const float size);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param colour Font colour.
			///
			void update(std::string_view text, const graphics::Colour& colour);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param colour Font colour.
			///
			void update(std::string_view text, const float size, const graphics::Colour& colour);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param alignment Text alignment.
			///
			void update(std::string_view text, const Alignment alignment);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param alignment Text alignment.
			///
			void update(std::string_view text, const float size, const Alignment alignment);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param colour Font colour.
			/// \param alignment Text alignment.
			///
			void update(std::string_view text, const graphics::Colour& colour, const Alignment alignment);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param colour Font colour.
			/// \param alignment Text alignment.
			///
			void update(std::string_view text, const float size, const graphics::Colour& colour, const Alignment alignment);

			///
			/// \brief Get text width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const;

			///
			/// \brief Get text height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const;

			///
			/// \brief Get text.
			///
			/// Is cached for performance.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_text() const;

			///
			/// \brief Get text em size.
			///
			/// Is cached for performance.
			///
			/// \return Float as EM.
			///
			[[nodiscard]] float get_size() const;

			///
			/// Get the text alignment.
			///
			/// \return Alignment enum.
			///
			[[nodiscard]] Alignment get_alignment() const;

			///
			/// Get font ID.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_font() const;

			///
			/// Get VAO object.
			///
			/// \return Reference to this renderable's VAO.
			///
			[[nodiscard]] const graphics::VertexArray& get_vao() const override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Text(const Text&) = delete;

			///
			/// Copy assignment operator.
			///
			Text& operator=(const Text&) = delete;

		public:
			///
			/// Text colour.
			///
			graphics::Colour m_colour;

		private:
			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Font render texture.
			///
			graphics::RenderTexture m_rt;

			///
			/// Font Name.
			///
			std::string m_font_name;

			///
			/// Pointer to font resource.
			///
			std::shared_ptr<graphics::Font> m_font;

			///
			/// Cached text width.
			///
			float m_width;

			///
			/// Cached text height.
			///
			float m_height;

			///
			/// Text.
			///
			std::string m_text;

			///
			/// EM size.
			///
			float m_size;

			///
			/// Alignment.
			///
			Alignment m_alignment;
		};
	} // namespace components
} // namespace galaxy

#endif