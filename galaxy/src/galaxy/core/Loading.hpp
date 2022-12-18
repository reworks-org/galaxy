///
/// Loading.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADING_HPP_
#define GALAXY_CORE_LOADING_HPP_

#include "galaxy/meta/Async.hpp"
#include "galaxy/ui/RMLRenderer.hpp"

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
			/// Argument constructor.
			///
			/// \param doc UI document to display when loading.
			///
			Loading(const std::string& doc);

			///
			/// Destructor.
			///
			~Loading();

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
			/// Constructor.
			///
			Loading() = delete;

		private:
			///
			/// Handle to thread loading assets.
			///
			std::future<void> m_thread_handle;

			///
			/// RML context.
			///
			Rml::Context* m_context;

			///
			/// RML rendering interface.
			///
			ui::RMLRenderer m_renderer;
		};
	} // namespace core
} // namespace galaxy

#endif