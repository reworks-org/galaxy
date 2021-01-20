///
/// StaticBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "StaticBody.hpp"

namespace galaxy
{
	namespace physics
	{
		StaticBody::StaticBody(const float density, const glm::vec2& size) noexcept
		    : Body {density, size}
		{
		}

		StaticBody::StaticBody(const float density, const float width, const float height) noexcept
		    : Body {density, width, height}
		{
		}

		StaticBody::StaticBody(const nlohmann::json& json)
		    : Body {json.at("density"), json.at("width"), json.at("height")}
		{
		}

		const glm::vec2& StaticBody::get_vel() const noexcept
		{
			static const constexpr glm::vec2 s_vel = {0.0f, 0.0f};
			return s_vel;
		}

		const bool StaticBody::is_rigid() const noexcept
		{
			return true;
		}
	} // namespace physics
} // namespace galaxy