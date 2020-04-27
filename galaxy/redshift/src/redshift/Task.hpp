///
/// Task.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_TASK_HPP_
#define REDSHIFT_TASK_HPP_

#include <mutex>
#include <functional>

///
/// Core namespace.
///
namespace rs
{
	///
	/// A Task is an action (i.e. function call) to perform on another thread.
	/// Tasks run asynchronously.
	///
	class Task
	{
	public:
		///
		/// Default constructor.
		///
		Task();

		///
		/// Default destructor.
		///
		~Task();

		///
		/// \brief Set the task to perform.
		///
		/// Allowed to throw exceptions. Is mutex locked.
		///
		/// \param task Lamba, std::bind, etc containing code to run on seperate thread.
		///				The bool param is the check for completedness - set it once thread is done.
		///
		void setTask(const std::function<void(bool*)>& task);

		///
		/// Runs the task asynchronously on a new thread.
		///
		auto run();

		///
		/// Check if the task has completed or not.
		///
		/// \return True if completed.
		///
		const bool isCompleted() const noexcept;

	private:
		bool m_isCompleted;

		std::mutex m_mutex;
		std::function<void(bool*)> m_task;
	};
}

#endif