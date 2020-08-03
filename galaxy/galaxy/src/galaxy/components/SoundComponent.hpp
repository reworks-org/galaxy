///
/// SoundComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SOUNDCOMPONENT_HPP_
#define GALAXY_SOUNDCOMPONENT_HPP_

#include <frb/audio/Sound.hpp>
#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component representing an audio file.
	///
	struct SoundComponent final
	{
		///
		/// Constructor.
		///
		SoundComponent() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SoundComponent(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~SoundComponent();

		///
		/// Sound file.
		///
		frb::Sound m_sound;
	};
} // namespace galaxy

#endif