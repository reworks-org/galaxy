///
/// Layer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_LAYER_HPP_
#define STARMAP_LAYER_HPP_

#include "nlohmann/json_fwd.hpp"

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
		virtual ~Layer() = default;

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
	};
}

#endif