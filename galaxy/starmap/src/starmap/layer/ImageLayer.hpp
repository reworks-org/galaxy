///
/// ImageLayer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_IMAGELAYER_HPP_
#define STARMAP_IMAGELAYER_HPP_

#include "starmap/layer/Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Specialized type of layer.
	///
	class ImageLayer final : public starmap::Layer
	{
	public:
		///
		/// Constructor.
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
		~ImageLayer() noexcept;

		///
		/// Get image.
		///
		/// \return Image as std::string.
		///
		[[nodiscard]] std::string get_image() const noexcept;

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
} // namespace starmap

#endif