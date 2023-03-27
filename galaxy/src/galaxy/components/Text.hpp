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
			///
			void create(std::string_view text, const float size, const std::string& font, const graphics::Colour& colour, const int layer);

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
			/// Get font ID.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_font() const;

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

			///
			/// Configure renderable.
			///
			void configure() override;

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
			std::string m_font_id;

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
		};
	} // namespace components
} // namespace galaxy

#endif