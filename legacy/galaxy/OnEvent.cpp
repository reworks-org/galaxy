///
/// OnEvent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "OnEvent.hpp"

namespace galaxy
{
	namespace components
	{
		OnEvent::OnEvent() noexcept
		    : Serializable {this}
		{
		}

		OnEvent::OnEvent(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		OnEvent::OnEvent(OnEvent&& t) noexcept
		    : Serializable {this}
		{
		}

		OnEvent& OnEvent::operator=(OnEvent&& t) noexcept
		{
			if (this != &t)
			{
			}

			return *this;
		}

		nlohmann::json OnEvent::serialize()
		{
			nlohmann::json json = "{}"_json;

			return json;
		}

		void OnEvent::deserialize(const nlohmann::json& json)
		{
		}
	} // namespace components
} // namespace galaxy