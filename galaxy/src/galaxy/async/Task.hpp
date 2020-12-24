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
			Task();

			///
			/// Copy constructor.
			///
			Task(const Task&) = delete;

			///
			/// Copy assignment operator.
			///
			Task& operator=(const Task&) = delete;

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
	} // namespace async
} // namespace galaxy

#endif