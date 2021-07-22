///
/// PostProcessor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///
/// SMAA PORT: https://github.com/Asmodean-/SMAA-OpenGL
///

#ifndef GALAXY_GRAPHICS_POSTPROCESSOR_HPP_
#define GALAXY_GRAPHICS_POSTPROCESSOR_HPP_

#include "galaxy/graphics/postfx/SMAA.hpp"
#include "galaxy/graphics/postfx/Sharpen.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Manages post processing effects to apply to lit scene.
		///
		class PostProcessor final
		{
		public:
			///
			/// Constructor.
			///
			PostProcessor();

			///
			/// Destructor.
			///
			~PostProcessor();

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height);

			///
			/// Bind to draw to post processor framebuffer.
			///
			void bind() noexcept;

			///
			/// Unbind to draw to post processor framebuffer.
			///
			void unbind() noexcept;

			///
			/// Draw post effects to stored framebuffer.
			///
			void render_effects();

			///
			/// Draw finished post effects to default framebuffer (screen).
			///
			void render_output();

		private:
			///
			/// For geometry and lighting.
			///
			RenderTexture m_fb;

			///
			/// Simple output shader.
			///
			Shader m_output;

			///
			/// SMAA effect.
			///
			SMAA m_smaa;

			///
			/// Sharpening effect to counteract blurriness of AA filtering.
			///
			Sharpen m_sharpen;

			///
			/// Simple quad to draw when applying effects (buffer).
			///
			unsigned int m_screen_vbo;

			///
			/// Simple quad to draw when applying effects (array).
			///
			unsigned int m_screen_vao;

			///
			/// Output cache.
			///
			unsigned int m_output_fb;

			///
			/// Do SMAA pass.
			///
			bool m_do_smaa;

			///
			/// Do sharpening pass.
			///
			bool m_do_sharpen;
		};
	} // namespace graphics
} // namespace galaxy

#endif