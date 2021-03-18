///
/// Project.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "Project.hpp"

namespace sc
{
	Project::Project(std::string_view path) noexcept
	    : Serializable {this}, m_path {path}
	{
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
		return m_instance->get_stack().top();
	}

	nlohmann::json Project::serialize()
	{
		nlohmann::json json = "{\"instance\":{}}"_json;
		json["instance"]    = m_instance->serialize();

		return json;
	}

	void Project::deserialize(const nlohmann::json& json)
	{
		m_instance->deserialize(json.at("instance"));
	}
} // namespace sc