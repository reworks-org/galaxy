///
/// Layer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_LAYER_HPP_
#define STARMAP_LAYER_HPP_

#include "nlohmann/json_fwd.hpp"
#include "starmap/types/Property.hpp"

///
/// Core namespace.
///
namespace starmap
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
		/// \return height as a const int.
		///
		virtual const int getHeight() const noexcept final;

		///
		/// Get id - unique across all layers.
		///
		/// \return height as a const int.
		///
		virtual const int getID() const noexcept final;

		///
		/// Get layer name.
		///
		/// \return name as string.
		///
		virtual const std::string getName() const noexcept final;

		///
		/// Horizontal layer offset.
		///
		/// \return double.
		///
		virtual const double getOffsetX() const noexcept final;

		///
		/// Vertical layer offset.
		///
		/// \return double.
		///
		virtual const double getOffsetY() const noexcept final;

		///
		/// Opacity of layer.
		///
		/// \return double between 0 and 1.
		///
		virtual const double getOpacity() const noexcept final;

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

	protected:
		///
		/// Default constructor.
		///
		Layer() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure containing chunk array from root map.
		///
		explicit Layer(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		Layer(const Layer&) noexcept = default;

		///
		/// Move constructor.
		///
		Layer(Layer&&) noexcept = default;

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
		double m_offsetX;

		///
		/// Vertical layer offset in pixels.
		///
		double m_offsetY;

		///
		/// Value between 0 and 1.
		///
		double m_opacity;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;
	};

	template<typename T>
	inline const T Layer::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].getValue<T>();
	}
}

#endif