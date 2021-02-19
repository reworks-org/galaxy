///
/// ObjectLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_LAYER_OBJECTLAYER_HPP_
#define GALAXY_MAP_LAYER_OBJECTLAYER_HPP_

#include "galaxy/map/layer/Layer.hpp"
#include "galaxy/map/types/Object.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Specialized type of layer.
		///
		class ObjectLayer final : public Layer
		{
		public:
			///
			/// \brief Default constructor.
			///
			ObjectLayer();

			///
			/// \brief Parse constructor.
			///
			/// Does not call ObjectLayer::parse() you must call that afterwards.
			/// Parses Layer common json.
			///
			/// \param json JSON structure containing chunk array from root map.
			///
			explicit ObjectLayer(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~ObjectLayer();

			///
			/// Get draw order.
			///
			/// \return Draw order as std::string.
			///
			[[nodiscard]] std::string get_compression() const;

			///
			/// Get objects.
			///
			/// \return Std::vector of objects.
			///
			[[nodiscard]] const auto& get_objects() const;

		private:
			///
			/// topdown (default) or index.
			///
			std::string m_draw_order;

			///
			/// Array of objects.
			///
			std::vector<Object> m_objects;
		};
	} // namespace map
} // namespace galaxy

#endif