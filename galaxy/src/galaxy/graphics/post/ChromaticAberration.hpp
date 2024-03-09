///
/// ChromaticAberration.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_CHROMATICABERRATION_HPP_
#define GALAXY_GRAPHICS_POST_CHROMATICABERRATION_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

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

			///
			/// Set chromatic r offset.
			///
			/// \param r -1.0f to 1.0f. Red.
			///
			void set_r_offset(const float r);

			///
			/// Set chromatic r offset.
			///
			/// \param g -1.0f to 1.0f. Green.
			///
			void set_g_offset(const float g);

			///
			/// Set chromatic r offset.
			///
			/// \param b -1.0f to 1.0f. Blue.
			///
			void set_b_offset(const float b);

			///
			/// Set chromatic rgb offset.
			///
			/// \param r -1.0f to 1.0f. Red.
			/// \param g -1.0f to 1.0f. Green.
			/// \param b -1.0f to 1.0f. Blue.
			///
			void set_rgb_offset(const float r, const float g, const float b);

			///
			/// Is this effect enabled?
			///
			/// \return True if effect is enabled. Otherwise false.
			///
			[[nodiscard]] bool is_enabled() override;

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

			///
			/// Effect r colour offset.
			///
			float m_r_offset;

			///
			/// Effect g colour offset.
			///
			float m_g_offset;

			///
			/// Effect b colour offset.
			///
			float m_b_offset;
		};
	} // namespace graphics
} // namespace galaxy

#endif
