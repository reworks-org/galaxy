///
/// FramebufferRenderer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FRAMEBUFFERRENDERER_HPP_
#define QUASAR_FRAMEBUFFERRENDERER_HPP_

#include "qs/graphics/Sprite.hpp"
#include "qs/core/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec.
	///
	class Shader;

	///
	/// OpenGL 2D FramebufferRenderer for applying shaders to framebuffers.
	/// I.e. Renders sprites to a framebuffer.
	///
	class FramebufferRenderer final
	{
	public:
		///
		/// Constructor.
		///
		/// \param window Pointer to window.
		///
		FramebufferRenderer(qs::Window* window) noexcept;

		///
		/// Default destructor.
		///
		~FramebufferRenderer() noexcept = default;

		///
		/// \brief Start rendering to FramebufferRenderer.
		///
		/// Do not call this inside window::begin() / window::end().
		///
		void begin() noexcept;

		///
		/// \brief Draw sprite to post effect renderer.
		///
		/// Call between begin() and end().
		///
		/// \param sprite Sprite to draw to screen.
		/// \param shader Shader to apply to sprite. You must have called bind() already!
		///
		void submit(qs::Sprite& sprite, qs::Shader& shader) noexcept;

		///
		/// \brief Finish rendering.
		///
		/// Do not call this inside window::begin() / window::end().
		///
		void end() noexcept;

		///
		/// \brief Renders output to screen.
		///
		/// This MUST be called INSIDE window::begin() / window::end().
		///
		/// \param postEffect Shader to apply to sprite. You must have called bind() already!
		///
		void render(qs::Shader& postEffect) noexcept;

	private:
		///
		/// Pointer to window.
		///
		qs::Window* m_window;

		///
		/// Render Texture.
		///
		qs::RenderTexture m_framebuffer;

		///
		/// Sprite for texture.
		///
		qs::Sprite m_sprite;
	};
}

#endif