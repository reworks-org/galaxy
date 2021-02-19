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
		/// Specialized type of layer.
		///
		class ImageLayer final : public Layer
		{
		public:
			///
			/// \brief Constructor.
			///
			/// Throws a runtime exception if called.
			///
			ImageLayer();

			///
			/// \brief Parse constructor.
			///
			/// Does not call ImageLayer::parse() you must call that afterwards.
			/// Parses Layer common json.
			///
			/// \param json JSON structure containing chunk array from root map.
			///
			explicit ImageLayer(const nlohmann::json& json);

			///
			/// Destructor.
			///
			virtual ~ImageLayer() noexcept = default;

			///
			/// Get image.
			///
			/// \return Image as std::string.
			///
			[[nodiscard]] const std::string& get_image() const noexcept;

		private:
			///
			/// Image used by this layer.
			///
			std::string m_image;

			///
			/// Hex-formatted color (#RRGGBB).
			///
			std::string m_transparent_colour;
		};
	} // namespace map
} // namespace galaxy

#endif