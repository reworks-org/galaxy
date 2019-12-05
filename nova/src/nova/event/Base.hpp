///
/// EventBase.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef EVENTBASE_NOVA_HPP_
#define EVENTBASE_NOVA_HPP_

#include <any>
#include <cstddef>

namespace nova
{
	struct EventBase
	{
		virtual ~EventBase() = default;

		virtual void trigger(const std::any& event) = 0;
	};
}

#endif