///
/// AudioComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIOCOMPONENT_HPP_
#define GALAXY_AUDIOCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <frb/audio/Audible.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component representing an audio file.
	///
	class AudioComponent final
	{
	public:
		///
		/// Constructor.
		///
		AudioComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		AudioComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~AudioComponent() noexcept;

	private:
		///
		/// Audio file. 
		///
		frb::Audible m_audio;
	};
}

#endif