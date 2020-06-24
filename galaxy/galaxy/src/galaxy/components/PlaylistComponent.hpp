///
/// PlaylistComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLAYLISTCOMPONENT_HPP_
#define GALAXY_PLAYLISTCOMPONENT_HPP_

#include <nlohmann/json.hpp>
#include <frb/audio/Playlist.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// A playlist of audio files. Max of 8.
	///
	struct PlaylistComponent final
	{
		///
		/// Constructor.
		///
		PlaylistComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		PlaylistComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~PlaylistComponent() noexcept;

		///
		/// Playlist of audio.
		///
		frb::Playlist m_playlist;
	};
}

#endif