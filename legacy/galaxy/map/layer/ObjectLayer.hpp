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
		/// Specialized type of layer for objects.
		///
		class ObjectLayer final : public Layer
		{
		public:
			///
			/// Parse constructor.
			///
			/// \param json JSON structure containing chunk array from root map.
			/// \param zlevel Rendering level of this layer.
			///
			explicit ObjectLayer(const nlohmann::json& json, const int zlevel);

			///
			/// Destructor.
			///
			virtual ~ObjectLayer() noexcept;

			///
			/// Get draw order.
			///
			/// \return Draw order as std::string.
			///
			[[nodiscard]] const std::string& get_draworder() const noexcept;

			///
			/// Get objects colour.
			///
			/// \return Const graphics::Colour object.
			///
			[[nodiscard]] const graphics::Colour& get_colour() const noexcept;

			///
			/// Get objects.
			///
			/// \return Std::vector of objects.
			///
			[[nodiscard]] const std::vector<Object>& get_objects() const noexcept;

		private:
			///
			/// Constructor.
			///
			ObjectLayer() = delete;

		private:
			///
			/// Topdown (default) or index.
			///
			std::string m_draw_order;

			///
			/// Colour of objects.
			///
			graphics::Colour m_colour;

			///
			/// Array of objects.
			///
			std::vector<Object> m_objects;
		};
	} // namespace map
} // namespace galaxy

#endif