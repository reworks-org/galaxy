///
/// Task.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TASK_HPP_
#define PROTOSTAR_TASK_HPP_

#include <functional>

#include "protostar/async/ProtectedArithmetic.hpp"

///
/// Core namespace.
///
namespace protostar
{
	///
	/// A task to run on a thread.
	///
	class Task final
	{
	public:
		///
		/// Constructor.
		///
		Task() noexcept;

		///
		/// Destructor.
		///
		~Task() noexcept;

		///
		/// Set the task to be done.
		///
		/// \param function Lambda or function to call when task is executed.
		/// \param args ParameterPack of variable amount of arguments to call with function.
		///
		template<typename Func, typename... Args>
		void set(Func function, Args&& ...args);

		///
		/// Run the task on the thread.
		///
		/// \param threadPoolFinished Pointer to ThreadPool::m_isActive. If this is false your function should exit to prevent deadlocks!
		///
		void exec(const protostar::ProtectedBool* threadPoolFinished);

		///
		/// Blocks calling thread until this task is finished.
		///
		void waitUntilFinished() noexcept;

	private:
		///
		/// Stores task to be executed.
		///
		std::function<void(const protostar::ProtectedBool*)> m_task;

		///
		/// Check to make sure task is finished.
		///
		protostar::ProtectedBool m_isFinished;
	};

	template<typename Func, typename ...Args>
	inline void Task::set(Func function, Args&& ...args)
	{
		m_task = std::bind(function, args...);
	}
}

#endif