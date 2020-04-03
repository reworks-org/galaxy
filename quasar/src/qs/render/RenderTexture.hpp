///
/// RenderTexture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTURE_HPP_
#define QUASAR_RENDERTEXTURE_HPP_

#include "qs/core/Texture.hpp"
#include "qs/core/Shader.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Framebuffer texture. You render to this texture instead of the window.
	///
	class RenderTexture final
	{
	public:
		///
		/// Argument constructor.
		///
		/// \param w Width of the RenderTexture.
		/// \param h Height of the RenderTexture.
		///
		explicit RenderTexture(const int w, const int h);

		///
		/// Virtual destructor override.
		///
		~RenderTexture() override;

		///
		/// Activate RenderTexture for drawing.
		///
		void activate() noexcept;

		///
		/// Deactivate RenderTexture.
		///
		void deactivate() noexcept;

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

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		const int getWidth() const noexcept;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		const int getHeight() const noexcept;

	private:
		///
		/// Default constructor.
		///
		RenderTexture() noexcept = delete;

	private:
		///
		/// OpenGL framebuffer id.
		///
		unsigned int m_framebufferID;

		///
		/// OpenGL texture id.
		///
		unsigned int m_textureID;

		///
		/// OpenGL depth buffer id.
		///
		unsigned int m_depthBufferID;

		///
		/// Cached texture width.
		///
		int m_width;

		///
		/// Cached texture height.
		///
		int m_height;

		///
		/// Internal shader for rendertexture.
		///
		qs::Shader m_shader;
	};
}

#endif