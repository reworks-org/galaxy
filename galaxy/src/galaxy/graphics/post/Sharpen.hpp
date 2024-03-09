///
/// Sharpen.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_SHARPEN_HPP_
#define GALAXY_GRAPHICS_POST_SHARPEN_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Sharpening effect.
		///
		/// Run after Anti-Aliasing.
		///
		class Sharpen final : public PostEffect
		{
		  public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			Sharpen(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~Sharpen() = default;

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
			/// Set sharpening amount.
			///
			/// \param amount 0.0 - 10.0.
			///
			void set_amount(const float amount);

			///
			/// Get sharpening amount.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_amount() const;

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
			Sharpen() = delete;

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
			/// Sharpening amount.
			///
			float m_amount;
		};
	} // namespace graphics
} // namespace galaxy

#endif
