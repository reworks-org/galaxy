///
/// RenderTexture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTURE_HPP_
#define QUASAR_RENDERTEXTURE_HPP_

#include <glm/gtc/matrix_transform.hpp>

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
		~RenderTexture() noexcept;

		///
		/// Activate RenderTexture for drawing.
		///
		/// \param shader Shader to supply when drawing. Must have a 'u_projection' uniform!
		///
		void activate(qs::Shader& shader) noexcept;

		///
		/// Display framebuffer to RenderTexture.
		///
		void deactivate() noexcept;

		///
		/// \brief Will not destroy opengl texture if rendertexture is destroyed.
		///
		/// Constructor defaults this to true.
		///
		/// \param preserveTexture True to prevent opengl texture from being destroyed.
		///
		void preserveTexture(const bool preserveTexture) noexcept;

		///
		/// Get reference to internal texture.
		///
		const unsigned int getTexture() const noexcept;

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
		/// Orthographic perspective of rendertexture.
		///
		glm::mat4 m_projection;

		///
		/// Texture to render to.
		/// OpenGL handle.
		///
		unsigned int m_texture;

		///
		/// OpenGL framebuffer id.
		///
		unsigned int m_framebufferID;

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
		/// Preserve texture flag.
		///
		bool m_preserveTexture;
	};
}

#endif