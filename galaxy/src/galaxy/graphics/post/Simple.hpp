///
/// Simple.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_SIMPLE_HPP_
#define GALAXY_GRAPHICS_POST_SIMPLE_HPP_

#include "galaxy/graphics/post/PostEffect.hpp"

namespace galaxy
{
	namespace posteffect
	{
		///
		/// Draws output straight to screen with no effects.
		///
		class Simple final : public graphics::PostEffect
		{
		public:
			///
			/// Constructor.
			///
			Simple() = default;

			///
			/// Destructor.
			///
			virtual ~Simple() = default;

		private:
			///
			/// \brief Set the appropriate uniforms for the shader.
			///
			/// Called internally in the derived effect, then called by Window.
			///
			void apply_uniforms() override;
		};
	} // namespace posteffect
} // namespace galaxy

#endif