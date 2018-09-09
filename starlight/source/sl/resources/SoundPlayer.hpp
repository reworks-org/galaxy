///
/// SoundPlayer.hpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SOUNDPLAYER_HPP_
#define STARLIGHT_SOUNDPLAYER_HPP_

#include "sl/audio/Sound.hpp"
#include "sl/types/ResourceCache.hpp"

namespace sl
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
		~SoundPlayer() override;

		///
		/// Change system sound effect volume.
		///
		/// \param volume Relative volume at which the sample is played; 1.0 is normal. Allegro also refers to this as gain.
		///
		void changeVolume(float volume);

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