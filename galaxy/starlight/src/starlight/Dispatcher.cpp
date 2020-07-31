///
/// Dispatcher.cpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#include "Dispatcher.hpp"

///
/// Core namespace.
///
namespace sl
{
	Dispatcher::Dispatcher()
	    : m_event_funcs {}
	{
	}

	Dispatcher::~Dispatcher()
	{
		m_event_funcs.clear();
	}
} // namespace sl