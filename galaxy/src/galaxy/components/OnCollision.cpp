///
/// OnCollision.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "OnCollision.hpp"

namespace galaxy
{
	namespace components
	{
		OnCollision::OnCollision() noexcept
			: Serializable {this}
		{
		}

		OnCollision::OnCollision(std::string_view script_id) noexcept
			: Serializable {this}
			, m_script {script_id}
		{
		}

		OnCollision::OnCollision(const nlohmann::json& json)
			: Serializable {this}
		{
			deserialize(json);
		}

		OnCollision::OnCollision(OnCollision&& t) noexcept
			: Serializable {this}
		{
			this->m_script = std::move(t.m_script);
		}

		OnCollision& OnCollision::operator=(OnCollision&& t) noexcept
		{
			if (this != &t)
			{
				this->m_script = std::move(t.m_script);
			}

			return *this;
		}

		nlohmann::json OnCollision::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["script"]      = m_script;

			return json;
		}

		void OnCollision::deserialize(const nlohmann::json& json)
		{
			m_script = json.at("script");
		}
	} // namespace components
} // namespace galaxy