///
/// Sharpen.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTEFFECTS_SHARPEN_HPP_
#define GALAXY_GRAPHICS_POSTEFFECTS_SHARPEN_HPP_

#include "galaxy/graphics/posteffects/PostEffect.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Sharpening effect.
		///
		class Sharpen final : public PostEffect
		{
		public:
			///
			/// Constructor.
			///
			Sharpen();

			///
			/// Virtual destructor.
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
			/// \return Returns output texture of SMAA'd scene.
			///
			[[nodiscard]] const unsigned int render(const unsigned int input) override;
		};
	} // namespace graphics
} // namespace galaxy

#endif