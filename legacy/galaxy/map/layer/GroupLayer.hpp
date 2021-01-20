///
/// GroupLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_LAYER_GROUPLAYER_HPP_
#define GALAXY_MAP_LAYER_GROUPLAYER_HPP_

#include "galaxy/map/layer/Layer.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Specialized type of layer.
		///
		class GroupLayer final : public Layer
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
			~GroupLayer();

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
			[[nodiscard]] const auto& get_layers() const;

		private:
			///
			/// Array of layers.
			///
			std::vector<std::unique_ptr<Layer>> m_layers;
		};
	} // namespace map
} // namespace galaxy

#endif