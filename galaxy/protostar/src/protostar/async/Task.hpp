///
/// Task.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TASK_HPP_
#define PROTOSTAR_TASK_HPP_

#include <atomic>
#include <functional>

///
/// Core namespace.
///
namespace pr
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
		Task();

		///
		/// Copy constructor.
		///
		Task(const Task&) = delete;

		///
		/// Move constructor.
		///
		Task(Task&&) = delete;

		///
		/// Copy assignment operator.
		///
		Task& operator=(const Task&) = delete;

		///
		/// Move assignment operator.
		///
		Task& operator=(Task&&) = delete;

		///
		/// Destructor.
		///
		~Task();

		///
		/// Set the task to be done.
		///
		/// \param func Lambda or function to call when task is executed. Is moved.
		///
		void set(std::function<void(void)>&& func);

		///
		/// Run the task on the thread.
		///
		void exec();

		///
		/// Blocks calling thread until this task is finished.
		///
		void wait_until_done();

		///
		/// Check if the task is finished.
		///
		/// \return Const bool. True if finished.
		///
		bool is_done();

	private:
		///
		/// Is function done?
		///
		std::atomic_bool m_done;

		///
		/// Stores task to be executed.
		///
		std::function<void(void)> m_task;
	};
} // namespace pr

#endif