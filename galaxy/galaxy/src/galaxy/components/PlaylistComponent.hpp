///
/// PlaylistComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLAYLISTCOMPONENT_HPP_
#define GALAXY_PLAYLISTCOMPONENT_HPP_

#include <frb/audio/Playlist.hpp>
#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// A playlist of audio files. Max of 8.
	///
	class PlaylistComponent final
	{
	public:
		///
		/// Constructor.
		///
		PlaylistComponent() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		PlaylistComponent(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~PlaylistComponent();
	};
} // namespace galaxy

#endif