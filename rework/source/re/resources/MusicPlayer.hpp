///
///  MusicPlayer.hpp
///  rework
///
///  Created by reworks on 12/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_MUSICPLAYER_HPP_
#define REWORK_MUSICPLAYER_HPP_

#include "re/audio/Music.hpp"
#include "re/types/ResourceCache.hpp"
#include "re/types/ServiceLocator.hpp"

namespace re
{
	class MusicPlayer final : public ServiceLocator<MusicPlayer>, public ResourceCache<Music>
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

		///
		/// Copy Constructor.
		/// Deleted.
		///
		MusicPlayer(const MusicPlayer&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		MusicPlayer(MusicPlayer&&) = delete;
	};
}

#endif