///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Loading.hpp"

namespace galaxy
{
	namespace core
	{
		Loading::Loading()
		{
		}

		Loading::~Loading() noexcept
		{
		}

		void Loading::start(const std::function<void(void)>& lambda)
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit(lambda);
		}

		void Loading::display()
		{
			while (!meta::is_work_done(m_thread_handle))
			{
				std::this_thread::sleep_for(0.5s);
			}

			m_thread_handle.get();
		}
	} // namespace core
} // namespace galaxy