///
/// SoundPlayer.hpp
/// galaxy
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SOUNDPLAYER_HPP_
#define GALAXY_SOUNDPLAYER_HPP_

#include "galaxy/audio/Sound.hpp"
#include "galaxy/types/ResourceCache.hpp"

galaxy
{
	///
	/// Manages all the sound effects the library uses.
	///
	class SoundPlayer final : public ResourceCache<Sound>
	{
	public:
		///
		/// \brief Construct SoundPlayer and add sound.
		///
		/// \param script Script file containing sound data to parse. You do not need to append folder path to
		///        the sound file paths, done for you by engine using paths in config.
		///
		explicit SoundPlayer(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~SoundPlayer() noexcept override;

		///
		/// Change system sound effect volume.
		///
		/// \param volume Relative volume at which the sample is played; 1.0 is normal. Allegro also refers to this as gain. 0.0f minimum. 1.0f is normal. 2.0f is double, etc.
		///
		void changeVolume(const float volume);

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		SoundPlayer() = delete;

	public:
		///
		/// The null sound file name.
		///
		std::string m_nullSound;
	};
}

#endif