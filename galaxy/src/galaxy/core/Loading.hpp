///
/// Loading.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADING_HPP_
#define GALAXY_CORE_LOADING_HPP_

#include "galaxy/meta/Async.hpp"

struct GLFWwindow;

namespace galaxy
{
	namespace core
	{
		///
		/// Simple customizable loading screen.
		///
		class Loading final
		{
		public:
			///
			/// Constructor.
			///
			Loading();

			///
			/// Destructor.
			///
			~Loading() noexcept;

			///
			/// Start loading job in task pool.
			///
			/// \param lambda Task to call in thread pool.
			///
			void start(const std::function<void(void)>& lambda);

			///
			/// Draw loading screen while loading.
			///
			void display();

		private:
			///
			/// Handle to thread loading assets.
			///
			std::future<void> m_thread_handle;
		};
	} // namespace core
} // namespace galaxy

#endif