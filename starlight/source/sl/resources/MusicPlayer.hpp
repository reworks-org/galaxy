///
/// MusicPlayer.hpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_MUSICPLAYER_HPP_
#define STARLIGHT_MUSICPLAYER_HPP_

#include "sl/audio/Music.hpp"
#include "sl/types/ResourceCache.hpp"

namespace sl
{
	///
	/// Manages all the music the library uses.
	///
	class MusicPlayer final : public ResourceCache<Music>
	{
	public:
		///
		/// \brief Construct MusicPlayer and add music.
		///
		/// \param script Script file containing sound/music data to parse.
		///
		MusicPlayer(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~MusicPlayer() override;

		///
		/// Change system music volume.
		///
		/// \param volume Relative volume at which the sample is played; 1.0 is normal. Allegro also refers to this as gain.
		///
		void changeMusicVolume(float volume);

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		MusicPlayer() = delete;
	};
}

#endif