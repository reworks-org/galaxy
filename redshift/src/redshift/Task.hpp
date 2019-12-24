///
/// Task.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_TASK_HPP_
#define REDSHIFT_TASK_HPP_

#include <mutex>

namespace rs
{
	class Task
	{
	public:

	private:
		std::lock_guard<std::mutex> m_lock;
	};
}

#endif