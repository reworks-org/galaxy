///
/// PostEffect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_POSTEFFECT_HPP_
#define GALAXY_GRAPHICS_POST_POSTEFFECT_HPP_

#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Post effect to apply to final image output.
		///
		class PostEffect
		{
			friend class Window;

		public:
			///
			/// Default destructor.
			///
			virtual ~PostEffect() = default;

			///
			/// Shader.
			///
			Shader m_shader;

		protected:
			///
			/// Default constructor.
			///
			PostEffect() = default;

		private:
			///
			/// \brief Set the appropriate uniforms for the shader.
			///
			/// Called internally in the derived effect, then called by Window.
			///
			virtual void apply_uniforms() = 0;
		};
	} // namespace graphics
} // namespace galaxy

#endif