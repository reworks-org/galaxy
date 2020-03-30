///
/// Texture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <string>

#include "qs/core/Colours.hpp"

///
/// Core namespace.
///
namespace qs
{
	class Texture final
	{
	public:
		///
		/// Enum contaning GL texture filtering options.
		///
		enum class Filter : int
		{
			///
			/// Produces blocky/pixellated texture patterns.
			/// Think 8-bit.
			///
			NEAREST,

			///
			/// Attempts to smooth texture pattern. Less visible pixels.
			///
			LINEAR,

			///
			/// Takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
			///
			NEAREST_MIPMAP_NEAREST,

			///
			/// Takes the nearest mipmap level and samples that level using linear interpolation.
			///
			LINEAR_MIPMAP_NEAREST,

			///
			/// Linearly interpolates between the two mipmaps that most closely match the size of a pixel 
			/// and samples the interpolated level via nearest neighbor interpolation.
			///
			NEAREST_MIPMAP_LINEAR,

			///
			/// Linearly interpolates between the two closest mipmaps and samples the interpolated 
			/// level via linear interpolation.
			///
			LINEAR_MIPMAP_LINEAR
		};

		Texture(const std::string& file, bool repeat = false, const std::array<float, 4>& border = { 1.0f, 1.0f, 1.0f, 0.0f });
		~Texture() noexcept;

		void bind() noexcept;
		void unbind() noexcept;

		void setRepeated() noexcept;
		void setMirrored() noexcept;
		void clampToEdge() noexcept;
		void clampToBorder(protostar::Colour& border) noexcept;

		void setMinifyFilter(const qs::Texture::Filter& filter);
		void setMagnifyFilter(const qs::Texture::Filter& filter);

	private:
		unsigned int m_id;
	};
}

#endif