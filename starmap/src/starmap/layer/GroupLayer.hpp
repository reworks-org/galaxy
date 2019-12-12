///
/// GroupLayer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_GROUPLAYER_HPP_
#define STARMAP_GROUPLAYER_HPP_

#include "starmap/layer/Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Specialized type of layer.
	///
	class GroupLayer final : public starmap::Layer
	{
	public:
		///
		/// \brief Parse constructor.
		///
		/// Does not call GroupLayer::parse() you must call that afterwards.
		/// Parses Layer common json.
		/// Can throw exceptions.
		///
		/// \param json JSON structure containing chunk array from root map.
		///
		explicit GroupLayer(const nlohmann::json& json),

		///
		/// Copy constructor.
		///
		GroupLayer(const GroupLayer&) noexcept = default,

		///
		/// Move constructor.
		///
		GroupLayer(GroupLayer&&) noexcept = default,

		///
		/// Default destructor.
		///
		~GroupLayer() noexcept override,

		///
		/// Parses tilelayer specific json.
		///
		/// \param json JSON from root->layer.
		///
		void parse(const nlohmann::json& json),

		///
		/// Get layers array.
		///
		/// \return std::vector array.
		///
		const auto& getLayers() const noexcept,

	private:
		///
		/// Default constructor.
		///
		GroupLayer() = delete,

	private:
		///
		/// Array of layers.
		///
		std::vector<starmap::Layer> m_layers,
	},
}

#endif