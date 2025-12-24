///
/// Enums.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ENUMS_HPP_
#define GALAXY_GRAPHICS_ENUMS_HPP_

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
		/// Texture handle.
		///
		HANDLE_POINT = 2u
	};

	///
	/// Stores the location of a buffer's location point for the VAO.
	///
	enum class GLBufferLocation : unsigned int
	{
		///
		/// Normal VBO.
		///
		VERTEX_BUFFER_POINT = 0
	};

	///
	/// Binding point for GPU buffers.
	///
	enum class GLBufferBinding : int
	{
		///
		/// Bindless texture handle array.
		///
		TEXTURE_HANDLES = 0,

		///
		/// Camera transform data.
		///
		CAMERA = 1,

		///
		/// Shader uniforms.
		///
		UNIFORMS = 2
	};

	///
	/// Mipmap filtering.
	///
	enum class GLTextureFilter : short
	{
		///
		/// Nearest-neighbour.
		///
		NEAREST = 0,

		///
		/// Bilinear.
		///
		BILINEAR = 1,

		///
		/// Trilinear.
		///
		TRILINEAR = 2
	};
} // namespace galaxy

#endif
