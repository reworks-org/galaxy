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
	/// Represents a tile/object/image layer.
	///
	template<typename CRTP>
	class Layer
	{
	public:
		///
		/// Destructor.
		///
		virtual ~Layer() = default;

		virtual CRTP* getDerived() noexcept final;
		virtual const int getHeight() const noexcept final;


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
	};

	template<typename CRTP>
	inline CRTP* Layer<CRTP>::getDerived() noexcept
	{
		return static_cast<CRTP*>(this);
	}

	template<typename CRTP>
	inline const int Layer<CRTP>::getHeight() const noexcept
	{
		return m_height;
	}

	template<typename CRTP>
	inline Layer<CRTP>::Layer() noexcept
		:m_height(0), m_id(0)
	{
	}

	template<typename CRTP>
	inline Layer<CRTP>::Layer(const nlohmann::json& json)
		:m_height(0), m_id(0)
	{
	}
}

#endif