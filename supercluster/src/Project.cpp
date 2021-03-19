///
/// Project.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/scripting/JSONUtils.hpp>

#include "Project.hpp"

namespace sc
{
	Project::Project() noexcept
	    : Serializable {this}, m_path {"projects/"}
	{
	}

	Project::Project(std::string_view path) noexcept
	    : Serializable {this}, m_path {path}
	{
	}

	Project::~Project()
	{
		m_game_instance = nullptr;
	}

	void Project::new_project(std::string_view path)
	{
		m_path          = static_cast<std::string>(path);
		m_game_instance = nullptr;
		m_game_instance = std::make_unique<Game>();
	}

	void Project::load(std::string_view path)
	{
		auto json = json::parse_from_disk(path);
		if (json != std::nullopt)
		{
			deserialize(json.value());
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to open json for project.");
		}
	}

	void Project::save()
	{
		json::save_to_disk(get_path(), serialize());
	}

	void Project::export_game()
	{
		// NEEDS TO BE CONSTEXPR or CONSTINIT.
		std::string m_cmake_path;
		std::string m_cmake_code;
		std::string m_main_code;
	}

	void Project::set_path(std::string_view path) noexcept
	{
		m_path = static_cast<std::string>(path);
	}

	const std::string& Project::get_path() const noexcept
	{
		return m_path;
	}

	std::shared_ptr<core::Scene> Project::get_top_scene()
	{
		return m_game_instance->get_stack().top();
	}

	nlohmann::json Project::serialize()
	{
		nlohmann::json json = "{\"instance\":{}}"_json;
		json["instance"]    = m_game_instance->serialize();

		return json;
	}

	void Project::deserialize(const nlohmann::json& json)
	{
		m_game_instance->deserialize(json.at("instance"));
	}
} // namespace sc