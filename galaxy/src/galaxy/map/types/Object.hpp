///
/// Object.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_OBJECT_HPP_
#define GALAXY_MAP_TYPES_OBJECT_HPP_

#include <robin_hood.h>

#include "galaxy/map/types/Text.hpp"
#include "galaxy/map/types/Point.hpp"
#include "galaxy/map/types/Property.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Object representation in tiled.
		///
		class Object final
		{
		public:
			///
			/// Object type enum.
			///
			enum class Type : int
			{
				ELLIPSE,
				POINT,
				POLYGON,
				RECT,
				POLYLINE,
				TEXT
			};

			///
			/// Constructor.
			///
			Object() noexcept;

			///
			/// Parse constructor.
			///
			/// \param json JSON structure/array containing object.
			///
			explicit Object(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Object() noexcept;

			///
			/// Parse object level json.
			///
			/// \param json JSON structure/array containing object.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get object type enum.
			///
			/// \Return Const enum identifier.
			///
			[[nodiscard]] const Type get_type_enum() const noexcept;

			///
			/// Get global tile id, if object represents a tile.
			///
			/// \return Gid as const int.
			///
			[[nodiscard]] const int get_gid() const noexcept;

			///
			/// Get height of object.
			///
			/// \return Height in pixels as double.
			///
			[[nodiscard]] const double get_height() const noexcept;

			///
			/// Get object id.
			///
			/// \return Gid as const int.
			///
			[[nodiscard]] const int get_id() const noexcept;

			///
			/// Get name.
			///
			/// \return Name as const std::string&.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Gets points.
			///
			/// \return Points as std::vector array.
			///
			[[nodiscard]] const std::vector<Point>& get_points() const noexcept;

			///
			/// Retrieve property.
			/// You will need to provide the type when retrieving.
			///
			/// \param name Name of the property to retrieve.
			///
			/// \return Property cast as type.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type& get_property(std::string_view name);

			///
			/// Get rotation of object.
			///
			/// \return Double - angle in degrees clockwise.
			///
			[[nodiscard]] const double get_rotation() const noexcept;

			///
			/// Get template reference.
			///
			/// \return String reference to a template file, in case object is a template instance.
			///
			[[nodiscard]] const std::string& get_template() const noexcept;

			///
			/// Get text object (if object is text based).
			///
			/// \return Const reference to a text object.
			///
			[[nodiscard]] const Text& get_text() const noexcept;

			///
			/// Get type string.
			///
			/// \return String assigned to type field in editor.
			///
			[[nodiscard]] const std::string& get_type() const noexcept;

			///
			/// Get visibility.
			///
			/// \return True if object is visible.
			///
			[[nodiscard]] const bool is_visible() const noexcept;

			///
			/// Get width of object.
			///
			/// \return Width in pixels as double.
			///
			[[nodiscard]] const double get_width() const noexcept;

			///
			/// Get x pos of object.
			///
			/// \return The x coordinate of the object in pixels.
			///
			[[nodiscard]] const double get_x() const noexcept;

			///
			/// Get y pos of object.
			///
			/// \return The y coordinate of the object in pixels.
			///
			[[nodiscard]] const double get_y() const noexcept;

		private:
			///
			/// Object type identifier.
			///
			Type m_type_enum;

			///
			/// Global tile ID, only if object represents a tile.
			///
			int m_gid;

			///
			/// Height in pixels.
			///
			double m_height;

			///
			/// Incremental id, unique across all objects.
			///
			int m_id;

			///
			/// String assigned to name field in editor.
			///
			std::string m_name;

			///
			/// Array of Points, in case the object is a polygon/polyline
			///
			std::vector<Point> m_points;

			///
			/// Map of Properties.
			///
			robin_hood::unordered_flat_map<std::string, Property> m_properties;

			///
			/// Angle in degrees clockwise.
			///
			double m_rotation;

			///
			/// Reference to a template file, in case object is a template instance.
			///
			std::string m_template;

			///
			/// Only used for text objects.
			///
			Text m_text;

			///
			/// String assigned to type field in editor.
			///
			std::string m_type;

			///
			/// Whether object is shown.
			///
			bool m_visible;

			///
			/// Width in pixels.
			///
			double m_width;

			///
			/// X coordinate in pixels.
			///
			double m_x;

			///
			/// Y coordinate in pixels.
			///
			double m_y;
		};

		template<tiled_property Type>
		inline const Type& Object::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif