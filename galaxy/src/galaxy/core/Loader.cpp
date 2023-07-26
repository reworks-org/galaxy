///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Loader.hpp"

namespace galaxy
{
	namespace core
	{
		Loader::Loader()
		{
		}

		Loader::~Loader()
		{
		}

		void Loader::load(const std::function<void(void)>& loader_func)
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit(loader_func);

			while (!meta::is_work_done(m_thread_handle))
			{
				glfwPollEvents();
			}

			m_thread_handle.get();
		}
	} // namespace core
} // namespace galaxy