///
/// MusicComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MUSICCOMPONENT_HPP_
#define GALAXY_MUSICCOMPONENT_HPP_

#include <frb/audio/Music.hpp>
#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component representing an audio file.
	///
	class MusicComponent final
	{
	public:
		///
		/// Constructor.
		///
		MusicComponent() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		MusicComponent(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~MusicComponent();

		///
		/// Music file.
		///
		frb::Music m_music;
	};
} // namespace galaxy

#endif