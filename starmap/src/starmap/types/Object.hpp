///
/// Object.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_OBJECT_HPP_
#define STARMAP_OBJECT_HPP_

#include "nlohmann/json_fwd.hpp"
#include "starmap/types/Text.hpp"
#include "starmap/types/Point.hpp"
#include "starmap/types/Property.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Object representation in tiled.
	///
	class Object
	{
	public:
		///
		/// Default constructor.
		///
		Object() noexcept;

		///
		/// Copy constructor.
		///
		Object(const Object&) noexcept = default;

		///
		/// Move constructor.
		///
		Object(Object&&) noexcept = default;

		///
		/// Destructor.
		///
		~Object() noexcept;

		///
		/// \brief Parse object level jston.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing object.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get ellipse flag.
		///
		/// \return boolean true if ellipse.
		///
		const bool getEllipse() const noexcept;

		///
		/// Get gid.
		///
		/// \return gid as const int.
		///
		const int getGID() const noexcept;

		///
		/// Get name.
		///
		/// \return name as std::string.
		///
		const std::string& getName() const noexcept;

		///
		/// Get point flag.
		///
		/// \return boolean point flag is true.
		///
		const bool getPoint() const noexcept;

		///
		/// Gets points.
		///
		/// \return points as std::vector array.
		///
		const auto& getPoints() const noexcept;

		///
		/// Retrieve property.
		/// You will need to provide the type when retrieving.
		///
		/// \param name Name of the property to retrieve.
		///
		/// \return Property cast as type.
		///
		template<typename T>
		const T getProperty(const std::string& name) noexcept;

	private:
		///
		/// Used to mark an object as an ellipse.
		///
		bool m_ellipse;

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
		/// Used to mark an object as a point.
		///
		bool m_point;

		///
		/// Array of Points, in case the object is a polygon/polyline
		///
		std::vector<starmap::Point> m_points;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

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
		starmap::Text m_text;
	};

	template<typename T>
	inline const T Object::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].getValue<T>();
	}
}

#endif