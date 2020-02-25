///
/// TextureTooltip.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TEXTURETOOLTIP_HPP_
#define CELESTIAL_TEXTURETOOLTIP_HPP_

#include "celestial/interface/Texture.hpp"
#include "celestial/widgets/tooltip/Tooltip.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Displays text when hovering over a widget.
	/// Uses texture as background.
	///
	class TextureTooltip final : public celestial::Tooltip
	{
	public:
		///
		/// Constructor.
		///
		TextureTooltip() noexcept;

		///
		/// Destructor.
		///
		~TextureTooltip() noexcept;



	private:
		///
		/// Texture background.
		///
		TexturePtr m_texture;
	};
}

#endif