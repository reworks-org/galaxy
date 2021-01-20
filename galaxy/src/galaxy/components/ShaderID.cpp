///
/// ShaderID.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "ShaderID.hpp"

namespace galaxy
{
	namespace components
	{
		ShaderID::ShaderID()
		    : m_shader_id {"null"}
		{
		}

		ShaderID::ShaderID(std::string_view id)
		    : m_shader_id {id}
		{
		}

		ShaderID::ShaderID(const nlohmann::json& json)
		{
			m_shader_id = json.at("shader-id");
		}

		ShaderID::ShaderID(ShaderID&& sid) noexcept
		{
			this->m_shader_id = std::move(sid.m_shader_id);
		}

		ShaderID& ShaderID::operator=(ShaderID&& sid) noexcept
		{
			if (this != &sid)
			{
				this->m_shader_id = std::move(sid.m_shader_id);
			}

			return *this;
		}
	} // namespace components
} // namespace galaxy