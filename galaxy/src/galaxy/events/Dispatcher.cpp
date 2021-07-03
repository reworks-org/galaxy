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
		Dispatcher::Storage::Storage(Dispatcher::Storage&& s) noexcept
		{
			this->m_event_functions = std::move(s.m_event_functions);
		}

		Dispatcher::Storage& Dispatcher::Storage::operator=(Dispatcher::Storage&& s) noexcept
		{
			if (this != &s)
			{
				this->m_event_functions = std::move(s.m_event_functions);
			}

			return *this;
		}

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