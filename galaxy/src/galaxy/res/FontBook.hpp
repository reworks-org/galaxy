///
/// FontBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_FONTBOOK_HPP_
#define GALAXY_RES_FONTBOOK_HPP_

#include "galaxy/graphics/text/Font.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for fonts.
		///
		class FontBook final : public ResourceCache<graphics::Font>
		{
		public:
			///
			/// Constructor.
			///
			FontBook() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			FontBook(std::string_view file);

			///
			/// Destructor.
			///
			virtual ~FontBook() noexcept;

			///
			/// Create FontBook from JSON.
			///
			/// \param file JSON file to load.
			///
			void create_from_json(std::string_view file);

			///
			/// Clean up.
			///
			void clear() noexcept override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
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
			FontBook(const FontBook&) = delete;

			///
			/// Move constructor.
			///
			FontBook(FontBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			FontBook& operator=(const FontBook&) = delete;

			///
			/// Move assignment operator.
			///
			FontBook& operator=(FontBook&&) = delete;
		};
	} // namespace res
} // namespace galaxy

#endif