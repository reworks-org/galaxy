///
/// GaussianBlur.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_GAUSSIANBLUR_HPP_
#define GALAXY_GRAPHICS_POST_GAUSSIANBLUR_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Gaussian Blur pass.
		///
		/// Run after Anti-Aliasing.
		///
		class GaussianBlur final : public PostEffect
		{
		  public:
			///
			/// Pixel sample strength.
			///
			enum class Strength : int
			{
				///
				/// Pixel sample of 5.
				///
				LOW = 0,

				///
				/// Pixel sample of 9.
				///
				NORMAL = 1,

				///
				/// Pixel sample of 13.
				///
				STRONG = 2
			};

			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			GaussianBlur(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~GaussianBlur() = default;

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
			/// Set gaussian blur strength.
			///
			/// \param strength Enum of strength values.
			///
			void set_strength(const Strength strength);

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
			GaussianBlur() = delete;

		  private:
			///
			/// Shader for post processing effect.
			///
			Shader m_shader;

			///
			/// Framebuffer to render horizontal blur to.
			///
			RenderTexture m_horizontal;

			///
			/// Framebuffer to render vertical blur to.
			///
			RenderTexture m_vertical;

			///
			/// Which pixel sample strength to use.
			///
			Strength m_strength;
		};
	} // namespace graphics
} // namespace galaxy

#endif
