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
	///
	/// Holds an OpenGL texture information and data.
	///
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

		///
		/// \brief Default constructor.
		///
		/// Remember to call load()!
		///
		Texture() noexcept;

		///
		/// \brief Argument constructor.
		///
		/// Calls load().
		/// Can throw exceptions.
		///
		/// \param file File on disk to load from.
		///
		Texture(const std::string& file);

		///
		/// Destructor.
		///
		~Texture() noexcept;
		
		///
		/// \brief Loads texture from file.
		///
		/// Can throw exceptions.
		///
		/// \param file File on disk to load from.
		///
		void load(const std::string& file);

		///
		/// \brief Bind as active OpenGL texture.
		///
		/// Binds to GL_TEXTURE2D
		///
		void bind() noexcept;

		///
		/// \brief Unbinds texture from GL_TEXTURE2D context.
		///
		/// Note technically unbinds any texture since sets GL_TEXTURE2D to NULL.
		///
		void unbind() noexcept;

		///
		/// Makes the texture repeat over its verticies.
		///
		void setRepeated() noexcept;

		///
		/// Mirrors the texture.
		///
		void setMirrored() noexcept;

		///
		/// \brief Clamps texture to edges.
		///
		/// Clamps the coordinates between 0 and 1.
		/// The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		///
		void clampToEdge() noexcept;

		///
		/// \brief Clamps to the border.
		///
		/// Coordinates outside the range are now given a user-specified border color.
		///
		/// \param border Colour of the border.
		///
		void clampToBorder(protostar::Colour& border) noexcept;

		///
		/// Set filter when texture is downscaled in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMinifyFilter(const qs::Texture::Filter& filter);

		///
		/// Set filter when texture would be scaled up in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMagnifyFilter(const qs::Texture::Filter& filter);

	private:
		///
		/// Internal OpenGL ID of texture.
		///
		unsigned int m_id;
	};
}

#endif