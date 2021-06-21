///
/// Sharpen.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTFX_SHARPEN_HPP_
#define GALAXY_GRAPHICS_POSTFX_SHARPEN_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Sharpening effect.
		///
		class Sharpen final
		{
		public:
			///
			/// Constructor.
			///
			Sharpen();

			///
			/// Destructor.
			///
			~Sharpen() noexcept = default;

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height);

			///
			/// Render effect to input texture.
			///
			/// \return Returns output texture of SMAA'd scene.
			///
			[[nodiscard]] const unsigned int render(const unsigned int input);

		private:
			///
			/// Shader for post processing effect.
			///
			Shader m_shader;

			///
			/// Framebuffer to render effect to.
			///
			RenderTexture m_fb;
		};
	} // namespace graphics
} // namespace galaxy

#endif