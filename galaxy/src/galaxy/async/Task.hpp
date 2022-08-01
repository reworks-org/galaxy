///
/// Task.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_TASK_HPP_
#define GALAXY_ASYNC_TASK_HPP_

#include <atomic>
#include <functional>

namespace galaxy
{
	namespace async
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
			/// Set constructor.
			///
			/// \param func Lambda or function to call when task is executed.
			///
			Task(const std::function<void(void)>& func) noexcept;

			///
			/// Destructor.
			///
			~Task() noexcept;

			///
			/// Set the task to be done.
			///
			/// \param func Lambda or function to call when task is executed.
			///
			void set(const std::function<void(void)>& func) noexcept;

			///
			/// Run the task on the thread.
			///
			void exec() noexcept;

			///
			/// Blocks calling thread until this task is finished.
			///
			void wait_until_done() noexcept;

			///
			/// Check if the task is finished.
			///
			/// \return Const bool. True if finished.
			///
			[[nodiscard]] const bool is_done() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Task(const Task&) = delete;

			///
			/// Copy assignment operator.
			///
			Task& operator=(const Task&) = delete;

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
	} // namespace async
} // namespace galaxy

#endif