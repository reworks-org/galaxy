///
/// Task.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TASK_HPP_
#define PROTOSTAR_TASK_HPP_

#include <functional>

///
/// Core namespace.
///
namespace pr
{
	///
	/// Shorthand for task function type.
	///
	using FunctionCallback = std::function<void(void)>;

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
		/// \param func Lambda or function to call when task is executed. Is moved.
		///
		void set(FunctionCallback&& func) noexcept;

		///
		/// Run the task on the thread.
		///
		void exec() noexcept;

		///
		/// Blocks calling thread until this task is finished.
		///
		void waitUntilDone();

		///
		/// Check if the task is finished.
		///
		/// \return Const bool. True if finished.
		///
		const bool done() noexcept;

	private:
		///
		/// Is function done?
		///
		std::atomic_bool m_done;

		///
		/// Stores task to be executed.
		///
		FunctionCallback m_task;
	};
} // namespace pr

#endif