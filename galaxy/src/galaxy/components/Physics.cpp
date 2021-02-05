///
/// Physics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Physics.hpp"

namespace galaxy
{
	namespace components
	{
		Physics::Physics() noexcept
		{
		}
		Physics::Physics(const nlohmann::json& json)
		{
		}
		Physics::Physics(Physics&&) noexcept
		{
		}
		Physics& Physics::operator=(Physics&&) noexcept
		{
			// TODO: insert return statement here
		}
		Physics::~Physics()
		{
		}
	} // namespace components
} // namespace galaxy