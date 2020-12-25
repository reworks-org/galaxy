///
/// ShaderComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "ShaderComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ShaderComponent::ShaderComponent()
	{
	}

	ShaderComponent::ShaderComponent(const nlohmann::json& json)
	{
		auto path = galaxy::FileSystem::s_root + galaxy::FileSystem::s_shaders;
		if ((json.count("vertex-file") > 0) && (json.count("frag-file") > 0))
		{
			std::string vert = json.at("vertex-file");
			std::string frag = json.at("frag-file");
			m_shader.load_path(path + vert, path + frag);
		}
		else if ((json.count("vertex-string") > 0) && (json.count("frag-string") > 0))
		{
			std::string vert = json.at("vertex-string");
			std::string frag = json.at("frag-string");
			m_shader.load_raw(vert, frag);
		}
		else
		{
			PL_LOG(PL_ERROR, "No valid shader loading config found.");
		}
	}

	ShaderComponent::ShaderComponent(ShaderComponent&& sc)
	{
		this->m_shader = std::move(sc.m_shader);
	}

	ShaderComponent& ShaderComponent::operator=(ShaderComponent&& sc)
	{
		if (this != &sc)
		{
			this->m_shader = std::move(sc.m_shader);
		}

		return *this;
	}
} // namespace galaxy