///
/// Entity.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ECS_ENTITY_HPP_
#define GALAXY_ECS_ENTITY_HPP_

#include <cstdint>

namespace galaxy
{
	namespace ecs
	{
		///
		/// Entity typedef.
		///
		using Entity = std::uint64_t;
	} // namespace ecs
} // namespace galaxy

#endif