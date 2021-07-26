///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_LAYER_LAYER_HPP_
#define GALAXY_MAP_LAYER_LAYER_HPP_

#include <robin_hood.h>

#include "galaxy/map/types/Property.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Represents a tile/object/image/group layer.
		///
		class Layer
		{
		public:
			///
			/// Destructor.
			///
			virtual ~Layer() noexcept;

			///
			/// Get row count. Same as map height for fixed-size maps.
			///
			/// \return Height as a const int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get id - unique across all layers.
			///
			/// \return Height as a const int.
			///
			[[nodiscard]] const int get_id() const noexcept;

			///
			/// Get layer name.
			///
			/// \return Name as string.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Horizontal layer offset.
			///
			/// \return Const double.
			///
			[[nodiscard]] const double get_offset_x() const noexcept;

			///
			/// Vertical layer offset.
			///
			/// \return Const double.
			///
			[[nodiscard]] const double get_offset_y() const noexcept;

			///
			/// Opacity of layer.
			///
			/// \return Const double between 0.0 and 1.0.
			///
			[[nodiscard]] const double get_opacity() const noexcept;

			///
			/// \brief Retrieve property.
			///
			/// You will need to provide the type when retrieving.
			/// DO NOT OVERRIDE THIS METHOD.
			///
			/// \param name Name of the property to retrieve.
			///
			/// \return Property cast as type.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type& get_property(std::string_view name);

			///
			/// \brief Get X coordinate where layer content starts.
			///
			/// For infinite maps.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_start_x() const noexcept;

			///
			/// \brief Get Y coordinate where layer content starts.
			///
			/// For infinite maps.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_start_y() const noexcept;

			///
			/// Get colour object.
			///
			/// \return Const reference to graphics colour object.
			///
			[[nodiscard]] const graphics::Colour& get_tint_colour() const noexcept;

			///
			/// Get type of layer.
			///
			/// \return String in format: tilelayer, objectgroup, imagelayer or group.
			///
			[[nodiscard]] const std::string& get_type() const noexcept;

			///
			/// Get visibility of layer.
			///
			/// \return True if layer is visible.
			///
			[[nodiscard]] const bool is_visible() const noexcept;

			///
			/// Get width of layer.
			///
			/// \return Const int. Column count. Same as map width for fixed-size maps.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get Z level.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_z_level() const noexcept;

		protected:
			///
			/// Constructor.
			///
			Layer() noexcept;

			///
			/// \brief Parse constructor.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure containing chunk array from root map.
			/// \param zlevel Rendering level of this layer.
			///
			explicit Layer(const nlohmann::json& json, const int zlevel);

		protected:
			///
			/// Row count. Same as map height for fixed-size maps.
			///
			int m_height;

			///
			/// Incremental id - unique across all layers.
			///
			int m_id;

			///
			/// Name assigned to this layer.
			///
			std::string m_name;

			///
			/// Horizontal layer offset in pixels.
			///
			double m_offset_x;

			///
			/// Vertical layer offset in pixels.
			///
			double m_offset_y;

			///
			/// Value between 0 and 1.
			///
			double m_opacity;

			///
			/// Map of Properties.
			///
			robin_hood::unordered_flat_map<std::string, Property> m_properties;

			///
			/// X coordinate where layer content starts (for infinite maps).
			///
			int m_start_x;

			///
			/// Y coordinate where layer content starts (for infinite maps).
			///
			int m_start_y;

			///
			/// Colour object.
			///
			graphics::Colour m_tint_colour;

			///
			/// Type of layer in string.
			///
			std::string m_type;

			///
			/// Whether layer is shown or hidden in editor.
			///
			bool m_visible;

			///
			/// Column count. Same as map width for fixed-size maps.
			///
			int m_width;

			///
			/// Z level of layer.
			///
			int m_z_level;
		};

		template<tiled_property Type>
		inline const Type& Layer::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif