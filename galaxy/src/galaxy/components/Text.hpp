///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TEXT_HPP_
#define GALAXY_COMPONENTS_TEXT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/text/Text.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// String of glyphs rendered with a font.
		///
		class Text final : public fs::Serializable
		{
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
			/// Text object.
			///
			graphics::Text m_text;
		};
	} // namespace components
} // namespace galaxy

#endif
