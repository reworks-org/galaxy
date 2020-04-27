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
	/// Shorthand for task function type.
	///
	using TaskFunction = std::function<void(protostar::ProtectedBool*)>;

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
		/// \param function Lambda or function to call when task is executed. Is moved.
		///
		void set(const TaskFunction&& function);

		///
		/// Run the task on the thread.
		///
		/// \param threadPoolFinished Pointer to ThreadPool::m_isActive. If this is false your function should exit to prevent deadlocks!
		///
		void exec(protostar::ProtectedBool* threadPoolFinished);

		///
		/// Blocks calling thread until this task is finished.
		///
		void waitUntilFinished() noexcept;

	private:
		///
		/// Stores task to be executed.
		///
		TaskFunction m_task;

		///
		/// Check to make sure task is finished.
		///
		protostar::ProtectedBool m_isFinished;
	};
}

#endif