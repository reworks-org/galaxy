///
/// ShaderComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "ShaderComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ShaderComponent::ShaderComponent(const nlohmann::json& json)
	{
		if ((json.count("vertex-file") > 0) && (json.count("frag-file") > 0))
		{
			m_shader.load_path(json.at("vertex-file"), json.at("frag-file"));
		}
		else if ((json.count("vertex-string") > 0) && (json.count("frag-string") > 0))
		{
			m_shader.load_raw(json.at("vertex-string"), json.at("frag-string"));
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