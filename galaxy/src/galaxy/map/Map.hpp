///
/// Map.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_MAP_HPP_
#define GALAXY_MAP_MAP_HPP_

#include <LDtkLoader/Level.hpp>

#include "galaxy/core/Registry.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// \brief An LDTK level.
		///
		/// Some implementation notes:
		/// Only 1 tileset per layer is currently supported.
		/// AutoLayer is not supported at this time.
		///
		class Map final
		{
		  public:
			///
			/// Constructor.
			///
			Map();

			///
			/// Move constructor.
			///
			Map(Map&&);

			///
			/// Move assignment operator.
			///
			Map& operator=(Map&&);

			///
			/// Destructor.
			///
			~Map();

			///
			/// Load a map from an ldtk level.
			///
			/// \param level Parsed from an LDTK project.
			///
			void load(const ldtk::Level& level);

			///
			/// Get map name.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& name() const;

		  private:
			///
			/// Copy constructor.
			///
			Map(const Map&) = delete;

			///
			/// Copy assignment operator.
			///
			Map& operator=(const Map&) = delete;

			///
			/// Parse LDTK int-grid layer.
			///
			/// \param layer Layer being parsed.
			///
			void parse_intgrid_layer(const ldtk::Layer& layer);

			///
			/// Parse LDTK entity layer.
			///
			/// \param layer Layer being parsed.
			///
			void parse_entitiy_layer(const ldtk::Layer& layer);

			///
			/// Parse LDTK tile layer / auto layer.
			///
			/// \param layer Layer being parsed.
			///
			void parse_tile_layer(const ldtk::Layer& layer, const int render_layer);

		  public:
			///
			/// Map entities.
			///
			core::Registry m_registry;

		  private:
			///
			/// Map name.
			///
			std::string m_name;
		};
	} // namespace map
} // namespace galaxy

#endif
