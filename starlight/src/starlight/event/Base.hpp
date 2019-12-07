///
/// EventBase.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef EVENTBASE_STARLIGHT_HPP_
#define EVENTBASE_STARLIGHT_HPP_

#include <any>
#include <cstddef>

namespace starlight
{
	struct EventBase
	{
		virtual ~EventBase() = default;

		virtual void trigger(const std::any& event) = 0;
	};
}

#endif