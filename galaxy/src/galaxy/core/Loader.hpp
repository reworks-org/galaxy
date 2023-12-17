///
/// Loader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADER_HPP_
#define GALAXY_CORE_LOADER_HPP_

#include <entt/entity/fwd.hpp>

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
			/// \brief Loads resources, user config and window settings.
			///
			/// Does not load maps.
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
			/// Load maps.
			///
			/// \param maps List of maps to load.
			/// \param registry Entity registry to load maps into.
			///
			void load_maps(const std::vector<std::string>& maps, entt::registry& registry);

			///
			/// Builds opengl resources on the main thread.
			///
			void build_resources();
		};
	} // namespace core
} // namespace galaxy

#endif
