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
		Dispatcher::Dispatcher(Dispatcher&& d) noexcept
		{
			this->m_event_funcs = std::move(d.m_event_funcs);
		}

		Dispatcher& Dispatcher::operator=(Dispatcher&& d) noexcept
		{
			if (this != &d)
			{
				this->m_event_funcs = std::move(d.m_event_funcs);
			}

			return *this;
		}

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