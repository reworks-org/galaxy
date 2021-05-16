///
/// PostEffect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTEFFECTS_POSTEFFECT_HPP_
#define GALAXY_GRAPHICS_POSTEFFECTS_POSTEFFECT_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Superclass for post processing effects.
		///
		class PostEffect
		{
		public:
			///
			/// Constructor.
			///
			PostEffect();

			///
			/// Virtual default destructor.
			///
			virtual ~PostEffect() = default;

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			virtual void resize(const int width, const int height) = 0;

			///
			/// Render effect to input texture.
			///
			/// \return Returns output texture of SMAA'd scene.
			///
			[[nodiscard]] virtual const unsigned int render(const unsigned int input) = 0;

		protected:
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