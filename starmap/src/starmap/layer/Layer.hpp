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
		/// \return Height as a const int.
		///
		virtual const int getHeight() const noexcept final;

		///
		/// Get id - unique across all layers.
		///
		/// \return Height as a const int.
		///
		virtual const int getID() const noexcept final;

		///
		/// Get layer name.
		///
		/// \return Name as string.
		///
		virtual const std::string& getName() const noexcept final;

		///
		/// Horizontal layer offset.
		///
		/// \return Const double.
		///
		virtual const double getOffsetX() const noexcept final;

		///
		/// Vertical layer offset.
		///
		/// \return Const double.
		///
		virtual const double getOffsetY() const noexcept final;

		///
		/// Opacity of layer.
		///
		/// \return Double between 0 and 1.
		///
		virtual const double getOpacity() const noexcept final;

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
		template<typename T>
		const T getProperty(const std::string& name) noexcept;

		///
		/// Get X coordinate where layer content starts.
		///
		/// \return Const int.
		///
		virtual const int getStartX() const noexcept final;
		
		///
		/// Get Y coordinate where layer content starts.
		///
		/// \return Const int.
		///
		virtual const int getStartY() const noexcept final;

		///
		/// Get type of layer.
		///
		/// \return String in format: tilelayer, objectgroup, imagelayer or group.
		///
		virtual const std::string& getType() const noexcept final;

		///
		/// Get visibility of layer.
		///
		/// \return True if layer is visible.
		///
		virtual const bool isVisible() const noexcept final;

		///
		/// Get width of layer.
		///
		/// \return Const int. Column count. Same as map width for fixed-size maps.
		///
		virtual const int getWidth() const noexcept final;

		///
		/// Get X offset.
		///
		/// \return Const int. Horizontal layer offset in tiles.
		///
		virtual const int getX() const noexcept final;

		///
		/// Get Y offset.
		///
		/// \return Const int. Vertical layer offset in tiles.
		///
		virtual const int getY() const noexcept final;

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
		Layer(const Layer&) = default;

		///
		/// Move constructor.
		///
		Layer(Layer&&) = default;

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

		///
		/// X coordinate where layer content starts (for infinite maps).
		///
		int m_startx;

		///
		/// Y coordinate where layer content starts (for infinite maps).
		///
		int m_starty;

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
		/// Horizontal layer offset in tiles.
		///
		int m_x;

		///
		/// Vertical layer offset in tiles.
		///
		int m_y;
	};

	template<typename T>
	inline const T Layer::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif