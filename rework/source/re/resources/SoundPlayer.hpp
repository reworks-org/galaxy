///
///  SoundPlayer.hpp
///  rework
///
///  Created by reworks on 12/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SOUNDPLAYER_HPP_
#define REWORK_SOUNDPLAYER_HPP_

#include "re/audio/Sound.hpp"
#include "re/types/ResourceCache.hpp"
#include "re/types/ServiceLocator.hpp"

namespace re
{
	class SoundPlayer final : public ServiceLocator<SoundPlayer>, public ResourceCache<Sound>
	{
	public:
		///
		/// \brief Construct SoundPlayer and add sound.
		///
		/// \param script Script file containing sound data to parse.
		///
		SoundPlayer(const std::string& script);

		///
		/// Cleanup fonts.
		///
		~SoundPlayer() override;

		///
		/// Change system sound effect volume.
		///
		/// \param volume Relative volume at which the sample is played; 1.0 is normal. Allegro also refers to this as gain.
		///
		void changeSoundVolume(float volume);

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

		///
		/// Copy Constructor.
		/// Deleted.
		///
		SoundPlayer(const SoundPlayer&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		SoundPlayer(SoundPlayer&&) = delete;
	};
}

#endif