///
/// GammaCorrection.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_GAMMACORRECTION_HPP_
#define GALAXY_GRAPHICS_POST_GAMMACORRECTION_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief GammaCorrection effect.
		///
		/// Run after Anti-Aliasing.
		///
		class GammaCorrection final : public PostEffect
		{
		  public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			GammaCorrection(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~GammaCorrection() = default;

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
			/// Set gamma modifier.
			///
			/// \param gamma_mod You will need to lookup or tinker with value. The value will be used in the formula: 1.0 / amount. 0 defaults to 1.0.
			///
			void set_gamma(const float gamma_mod);

			///
			/// Get gamma modifier.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_gamma() const;

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
			GammaCorrection() = delete;

		  private:
			///
			/// Shader for post processing effect.
			///
			Shader m_shader;

			///
			/// Framebuffer to render effect to.
			///
			RenderTexture m_fb;

			///
			/// Amount to adjust gamma by.
			///
			float m_gamma;
		};
	} // namespace graphics
} // namespace galaxy

#endif
