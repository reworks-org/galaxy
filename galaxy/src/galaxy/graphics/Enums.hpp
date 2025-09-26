///
/// Enums.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_ENUMS_HPP_
#define GALAXY_GRAPHICS_GL_ENUMS_HPP_

namespace galaxy
{
	///
	/// Stores the location of shader attribute binding point for the VAO.
	///
	enum class GLAttributeBinding : unsigned int
	{
		///
		/// Position data.
		///
		POSITION_POINT = 0u,

		///
		/// Texel data.
		///
		TEXEL_POINT = 1u,

		///
		/// Instance offset data.
		///
		OFFSET_POINT = 2u
	};

	///
	/// Stores the location of a buffer's binding point for the VAO.
	///
	enum class GLBufferBinding : unsigned int
	{
		///
		/// Normal VBO.
		///
		VERTEX_BUFFER_POINT = 0,

		///
		/// Instance offsets.
		///
		INSTANCE_BUFFER_POINT = 1
	};

	///
	/// Mipmap filtering.
	///
	enum class GLTextureFilter : short
	{
		///
		/// Nearest-neighbour.
		///
		NEAREST,

		///
		/// Bilinear.
		///
		BILINEAR,

		///
		/// Trilinear.
		///
		TRILINEAR
	};
} // namespace galaxy

#endif
