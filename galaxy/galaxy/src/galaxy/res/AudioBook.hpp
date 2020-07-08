///
/// AudioBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIOBOOK_HPP_
#define GALAXY_AUDIOBOOK_HPP_

#include <frb/audio/Audible.hpp>
#include <protostar/system/ResourceCache.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Resource manager for fonts.
	///
	class AudioBook final : public protostar::ResourceCache<frb::Audible>
	{
	public:
		///
		/// Default constructor.
		///
		AudioBook() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON file to load.
		///
		explicit AudioBook(const std::string& json) noexcept;

		///
		/// Destructor.
		///
		~AudioBook() noexcept;

		///
		/// Create AudioBook from JSON.
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