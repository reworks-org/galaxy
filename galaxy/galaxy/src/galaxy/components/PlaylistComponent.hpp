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
	/// A playlist of audio files.
	///
	template<size_t length>
	class PlaylistComponent final
	{
	public:
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

	private:
		///
		/// Playlist of audio.
		///
		frb::Playlist<length> m_playlist;
	};
	
	template<size_t length>
	inline PlaylistComponent<length>::PlaylistComponent() noexcept
	{
	}

	template<size_t length>
	inline PlaylistComponent<length>::PlaylistComponent(const nlohmann::json& json) noexcept
	{
		std::vector<std::string> files;
		nlohmann::json arr = json.at("files");
		for (auto& elem : arr)
		{
			files.push_back(elem);
		}

		m_playlist.load(files);
		m_playlist.setLooping(json.at("looping"));
	}

	template<size_t length>
	inline PlaylistComponent<length>::~PlaylistComponent() noexcept
	{
	}
}

#endif