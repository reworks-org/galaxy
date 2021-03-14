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
			clear();
		}

		void Dispatcher::clear()
		{
			m_event_funcs.clear();
		}
	} // namespace events
} // namespace galaxy