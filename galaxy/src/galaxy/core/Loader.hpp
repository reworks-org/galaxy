///
/// Loading.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADING_HPP_
#define GALAXY_CORE_LOADING_HPP_

#include "galaxy/meta/Async.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Asset and Config loader.
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
			/// Extract a compressed asset archive.
			///
			void extract_assets();

			///
			/// Loads resources, user config and window settings.
			///
			void load_all();

			///
			/// Load game resources.
			///
			void load_resources();

			///
			/// Only loads user config.
			///
			void load_user_config();

			///
			/// Only loads the window settings.
			///
			void load_window();

			///
			/// Builds opengl resources on the main thread.
			///
			void build_resources();
		};
	} // namespace core
} // namespace galaxy

#endif
