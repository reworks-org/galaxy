///
/// MusicPlayer.hpp
/// galaxy
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MUSICPLAYER_HPP_
#define GALAXY_MUSICPLAYER_HPP_

#include "galaxy/audio/Music.hpp"
#include "galaxy/types/ResourceCache.hpp"

galaxy
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
		/// \param script Script file containing sound data to parse. You do not need to append folder path to
		///        the music file paths, done for you by engine using paths in config.
		///
		explicit MusicPlayer(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~MusicPlayer() noexcept override;

		///
		/// Change system music volume.
		///
		/// \param volume Relative volume at which the sample is played; 1.0 is normal. Allegro also refers to this as gain. 0.0f minimum. 1.0f is normal. 2.0f is double, etc.
		///
		void changeVolume(const float volume);

		///
		/// Play a song from the beginning.
		///
		/// \param music The music file to use.
		///
		void play(const std::string& music);

		///
		/// Stops a song.
		///
		/// \param music The music file to use.
		///
		void stop(const std::string& music);

		///
		/// Resumes a stopped song from the position where it stopped.
		///
		/// \param music The music file to use.
		///
		void resume(const std::string& music);

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

	public:
		///
		/// The null music file name.
		///
		std::string m_nullMusic;
	};
}

#endif