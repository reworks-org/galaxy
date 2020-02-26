///
/// ThreadPool.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_THREADPOOL_HPP_
#define PROTOSTAR_THREADPOOL_HPP_

#include <thread>
#include <vector>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// 
	///
	class ThreadPool
	{
	public:
		///
		/// Constructor.
		///
		ThreadPool() noexcept;

		///
		/// \brief Thread count constructor.
		///
		/// Can throw exceptions.
		///
		/// \param count Amount of threads to create for pool.
		///
		explicit ThreadPool(const std::size_t count);

		///
		/// Destructor.
		///
		~ThreadPool() noexcept;

		///
		/// \brief Creates a number of threads.
		///
		/// Can throw exceptions.
		///
		/// \param count Amount of threads to create for pool.
		///
		void create(const size_t count);

	private:
		///
		/// Maximum threads avaliable for pool.
		///
		std::size_t m_maxThreadCount;

		///
		///
		///
		std::vector<std::thread> m_workers;
	};
}

#endif