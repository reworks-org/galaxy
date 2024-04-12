///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_WORLD_HPP_
#define GALAXY_MAP_WORLD_HPP_

#include <ankerl/unordered_dense.h>
#include <LDtkLoader/Project.hpp>

#include "galaxy/map/Map.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// An LDTK level.
		///
		class World final
		{
		  public:
			///
			/// Constructor.
			///
			World();

			///
			/// Argument Constructor.
			///
			/// \param file File in VFS to load.
			///
			World(const std::string& file);

			///
			/// Destructor.
			///
			~World();

			///
			/// Load a world.
			///
			/// \param file World in VFS.
			///
			[[nodiscard]] bool load(const std::string& file);

			///
			/// Parse world and create entities.
			///
			void parse();

			///
			/// Set currently active map.
			///
			/// \param map Map ID.
			///
			void set_active(const std::string& map);

			///
			/// Get currently active map.
			///
			/// \return Pointer to active map.
			///
			[[nodiscard]] map::Map* get_active() const;

			///
			/// Get world name.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& name() const;

			///
			/// Get file.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& file() const;

		  private:
			///
			/// File.
			///
			std::string m_file;

			///
			/// LDTK project data.
			///
			ldtk::Project m_project;

			///
			/// List of ldtk levels.
			///
			ankerl::unordered_dense::map<std::string, map::Map> m_maps;

			///
			/// Current map.
			///
			map::Map* m_current;
		};
	} // namespace map
} // namespace galaxy

#endif
