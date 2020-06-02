///
/// PlaylistComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "PlaylistComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	PlaylistComponent::PlaylistComponent() noexcept
		:m_playlist()
	{
	}

	PlaylistComponent::PlaylistComponent(const nlohmann::json& json) noexcept
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

	PlaylistComponent::~PlaylistComponent() noexcept
	{
	}
}