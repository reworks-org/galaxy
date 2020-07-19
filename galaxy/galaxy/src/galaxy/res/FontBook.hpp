///
/// FontBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FONTBOOK_HPP_
#define GALAXY_FONTBOOK_HPP_

#include <qs/text/Font.hpp>
#include <protostar/system/ResourceCache.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Resource manager for fonts.
	///
	class FontBook final : public pr::ResourceCache<qs::Font>
	{
	public:
		///
		/// Default constructor.
		///
		FontBook() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON file to load.
		///
		explicit FontBook(const std::string& json) noexcept;

		///
		/// Destructor.
		///
		~FontBook() noexcept;

		///
		/// Create FontBook from JSON.
		///
		/// \param json JSON file to load.
		///
		void createFromJSON(const std::string& json);

		///
		/// Clean up.
		///
		void clear() noexcept override;
	};
}

#endif