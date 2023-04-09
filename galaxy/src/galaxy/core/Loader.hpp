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
		class Loader final
		{
		public:
			///
			/// Constructor.
			///
			Loader();

			///
			/// Destructor.
			///
			~Loader();

			///
			/// Start loading job in task pool.
			///
			void start();

			///
			/// Draw loading screen while loading.
			///
			void finish();

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