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
		/// \param shader Shader to supply when drawing.
		///
		void activate(qs::Shader& shader) noexcept;

		///
		/// Deactivate RenderTexture.
		///
		void deactivate() noexcept;

		///
		/// Get reference to internal texture.
		///
		qs::Texture& getTexture() noexcept;

		///
		/// Get std::move() compatible reference.
		///
		qs::Texture&& moveTexture() noexcept;

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
		///
		qs::Texture m_texture;

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
	};
}

#endif