///
/// Dispatcher.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Dispatcher.hpp"

namespace galaxy
{
	namespace events
	{
		Dispatcher::~Dispatcher()
		{
			m_event_funcs.clear();
		}
	} // namespace events
} // namespace galaxy