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
		    : Serializable {this}, m_shader_id {"null"}
		{
		}

		ShaderID::ShaderID(std::string_view id)
		    : Serializable {this}, m_shader_id {id}
		{
		}

		ShaderID::ShaderID(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		ShaderID::ShaderID(ShaderID&& sid) noexcept
		    : Serializable {this}
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

		nlohmann::json ShaderID::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["shader-id"]   = m_shader_id;

			return json;
		}

		void ShaderID::deserialize(const nlohmann::json& json)
		{
			m_shader_id = json.at("shader-id");
		}
	} // namespace components
} // namespace galaxy