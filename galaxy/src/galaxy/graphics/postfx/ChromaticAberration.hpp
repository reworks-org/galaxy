///
/// ChromaticAberration.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTFX_CHROMATICABERRATION_HPP_
#define GALAXY_GRAPHICS_POSTFX_CHROMATICABERRATION_HPP_

#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Chromatic Aberration pass.
		///
		/// Run after Anti-Aliasing.
		///
		class ChromaticAberration final : public PostEffect
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			ChromaticAberration(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~ChromaticAberration() = default;

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height) override;

			///
			/// Render effect to input texture.
			///
			/// \param input Input texture from previous effect or window framebuffer.
			///
			/// \return Returns output texture for next effect or final framebuffer.
			///
			[[nodiscard]] unsigned int render(const unsigned int input) override;

			void set_r_offset(const float r);
			void set_g_offset(const float g);
			void set_b_offset(const float b);
			void set_rgb_offset(const float r, const float g, const float b);

		private:
			///
			/// Constructor.
			///
			ChromaticAberration() = delete;

		private:
			///
			/// Shader for post processing effect.
			///
			Shader m_shader;

			///
			/// Framebuffer to render aberration.
			///
			RenderTexture m_fb;

			float m_r_offset;
			float m_g_offset;
			float m_b_offset;
		};
	} // namespace graphics
} // namespace galaxy

#endif