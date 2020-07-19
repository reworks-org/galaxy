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
namespace pr
{
	///
	/// Shorthand for task function type.
	///
	using FunctionCallback = std::function<void(pr::ProtectedBool*)>;

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
		void set(FunctionCallback&& function) noexcept;

		///
		/// Run the task on the thread.
		///
		/// \param isPoolDone Pointer to ThreadPool::m_isActive. If this is false your function should exit to
		/// prevent deadlocks!
		///
		void exec(pr::ProtectedBool* isPoolDone) noexcept;

		///
		/// Blocks calling thread until this task is finished.
		///
		void wait() noexcept;

		///
		/// Check if the task is finished.
		///
		/// \return Const bool. True if finished.
		///
		const bool isFinished() noexcept;

	private:
		///
		/// Stores task to be executed.
		///
		FunctionCallback m_task;

		///
		/// Check to make sure task is finished.
		///
		pr::ProtectedBool m_isFinished;
	};
} // namespace pr

#endif