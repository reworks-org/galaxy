///
/// ShaderComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ShaderComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ShaderComponent::ShaderComponent() noexcept
	{
	}

	ShaderComponent::ShaderComponent(const nlohmann::json& json) noexcept
	{
		if ((json.count("vertex-file") > 0) && (json.count("frag-file") > 0))
		{
			m_shader.loadFromPath(json.at("vertex-file"), json.at("frag-file"));
		}

		if ((json.count("vertex-string") > 0) && (json.count("frag-string") > 0))
		{
			m_shader.loadFromRaw(json.at("vertex-string"), json.at("frag-string"));
		}
	}
}