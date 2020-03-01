///
/// Task.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_TASK_HPP_
#define PROTOSTAR_TASK_HPP_

#include <functional>

#include "protostar/async/BaseTask.hpp"

///
/// Core namespace.
///
namespace protostar
{
	///
	/// A task to run on a thread.
	///
	class Task
	{
	public:
		///
		/// Make Task utility function.
		///
		/// \param function Lambda or function to call when task is executed.
		/// \param args ParameterPack of variable amount of arguments to call with function.
		///
		/// \return Returns a move constructed protostar::Task.
		///
		template<typename Func, typename... Args>
		static decltype(auto) makeTask(Func function, Args&& ...args);

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
		void exec();

	private:
		///
		/// Stores task to be executed.
		///
		std::function<void()> m_task;
	};

	template<typename Func, typename ...Args>
	inline decltype(auto) Task::makeTask(Func function, Args&& ...args)
	{
		Task task;
		task.set(function, ...args);

		return std::move(task);
	}

	template<typename Func, typename ...Args>
	inline void Task::set(Func function, Args&& ...args)
	{
		m_task = std::bind(function, args...);
	}
}

#endif