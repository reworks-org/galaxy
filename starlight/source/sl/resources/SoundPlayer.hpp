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
#include "sl/types/ServiceLocator.hpp"

namespace sl
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
	};
}

#endif