///
/// FilmicGrain.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_FilmicGrain_HPP_
#define GALAXY_GRAPHICS_POST_FilmicGrain_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Film grain effect.
		///
		/// Run after Anti-Aliasing.
		///
		class FilmicGrain final : public PostEffect
		{
		  public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			FilmicGrain(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~FilmicGrain() = default;

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
			/// Set intensity of film grain effect.
			///
			/// \param amount Lower value is less intense.
			///
			void set_amount(const float amount);

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
			FilmicGrain() = delete;

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
			/// Film grain intensity.
			///
			float m_amount;
		};
	} // namespace graphics
} // namespace galaxy

#endif
