///
/// EventBase.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef EVENTBASE_NOVA_HPP_
#define EVENTBASE_NOVA_HPP_

#include <cstddef>

namespace nova
{
	struct EventBase
	{
		virtual ~EventBase() = default;
		virtual void trigger() = 0;
	};
}

#endif