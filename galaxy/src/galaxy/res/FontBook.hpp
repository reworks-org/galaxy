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
			/// Default constructor.
			///
			FontBook() = default;

			///
			/// JSON constructor.
			///
			/// \param json JSON file to load.
			///
			explicit FontBook(std::string_view json);

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

			///
			/// Destructor.
			///
			virtual ~FontBook();

			///
			/// Create FontBook from JSON.
			///
			/// \param json JSON file to load.
			///
			void create_from_json(std::string_view json);

			///
			/// Clean up.
			///
			void clear() override;
		};
	} // namespace res
} // namespace galaxy

#endif