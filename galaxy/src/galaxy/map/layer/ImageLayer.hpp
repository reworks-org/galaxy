///
/// ImageLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_LAYER_IMAGELAYER_HPP_
#define GALAXY_MAP_LAYER_IMAGELAYER_HPP_

#include "galaxy/map/layer/Layer.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Specialized type of layer for images.
		///
		class ImageLayer final : public Layer
		{
		public:
			///
			/// Parse constructor.
			///
			/// \param json JSON structure containing chunk array from root map.
			/// \param zlevel Rendering level of this layer.
			///
			explicit ImageLayer(const nlohmann::json& json, const int zlevel);

			///
			/// Destructor.
			///
			virtual ~ImageLayer() noexcept = default;

			///
			/// Get image.
			///
			/// \return Image file as std::string.
			///
			[[nodiscard]] const std::string& get_image() const noexcept;

			///
			/// Get transparent colour.
			///
			/// \return Const reference to graphics::Colour.
			///
			[[nodiscard]] const graphics::Colour& get_transparent_colour() const noexcept;

		private:
			///
			/// Constructor.
			///
			ImageLayer() = delete;

		private:
			///
			/// Image used by this layer.
			///
			std::string m_image;

			///
			/// Transparent colour.
			///
			graphics::Colour m_transparent_colour;
		};
	} // namespace map
} // namespace galaxy

#endif