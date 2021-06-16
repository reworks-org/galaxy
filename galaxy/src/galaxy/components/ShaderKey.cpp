///
/// ShaderKey.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ShaderKey.hpp"

namespace galaxy
{
	namespace components
	{
		ShaderKey::ShaderKey()
		    : Serializable {this}, m_key {"null"}
		{
		}

		ShaderKey::ShaderKey(std::string_view id)
		    : Serializable {this}, m_key {id}
		{
		}

		ShaderKey::ShaderKey(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		ShaderKey::ShaderKey(ShaderKey&& sk) noexcept
		    : Serializable {this}
		{
			this->m_key = std::move(sk.m_key);
		}

		ShaderKey& ShaderKey::operator=(ShaderKey&& sk) noexcept
		{
			if (this != &sk)
			{
				this->m_key = std::move(sk.m_key);
			}

			return *this;
		}

		nlohmann::json ShaderKey::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["shader-key"]  = m_key;

			return json;
		}

		void ShaderKey::deserialize(const nlohmann::json& json)
		{
			m_key = json.at("shader-key");
		}
	} // namespace components
} // namespace galaxy