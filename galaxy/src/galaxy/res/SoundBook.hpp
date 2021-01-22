///
/// SoundBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_SOUNDBOOK_HPP_
#define GALAXY_RES_SOUNDBOOK_HPP_

#include "galaxy/audio/Sound.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for fonts.
		///
		class SoundBook final : public ResourceCache<audio::Sound>
		{
		public:
			///
			/// Constructor.
			///
			SoundBook() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			SoundBook(std::string_view file);

			///
			/// Destructor.
			///
			virtual ~SoundBook() noexcept;

			///
			/// Create SoundBook from JSON.
			///
			/// \param file JSON file to load.
			///
			void create_from_json(std::string_view file);

			///
			/// Clean up.
			///
			void clear() noexcept override;

		private:
			///
			/// Copy constructor.
			///
			SoundBook(const SoundBook&) = delete;

			///
			/// Move constructor.
			///
			SoundBook(SoundBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			SoundBook& operator=(const SoundBook&) = delete;

			///
			/// Move assignment operator.
			///
			SoundBook& operator=(SoundBook&&) = delete;
		};
	} // namespace res
} // namespace galaxy

#endif