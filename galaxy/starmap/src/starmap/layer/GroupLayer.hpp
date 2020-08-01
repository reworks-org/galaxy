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
		/// Constructor.
		///
		GroupLayer();

		///
		/// \brief Parse constructor.
		///
		/// Does not call GroupLayer::parse() you must call that afterwards.
		/// Parses Layer common json.
		///
		/// \param json JSON structure containing chunk array from root map.
		///
		explicit GroupLayer(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~GroupLayer() noexcept;

		///
		/// Parses tilelayer specific json.
		///
		/// \param json JSON from root->layer.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get layers array.
		///
		/// \return Std::vector array.
		///
		[[nodiscard]] const auto& get_layers() const noexcept;

	private:
		///
		/// Array of layers.
		///
		std::vector<std::unique_ptr<starmap::Layer>> m_layers;
	};
} // namespace starmap

#endif