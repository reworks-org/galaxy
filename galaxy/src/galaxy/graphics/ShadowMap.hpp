///
/// ShadowMap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADOWMAP_HPP_
#define GALAXY_GRAPHICS_SHADOWMAP_HPP_

#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Percentage-Close Soft Shadows.
		///
		class ShadowMap final
		{
		public:
			ShadowMap();
			~ShadowMap();

			///
			/// Clear framebuffer contents.
			///
			void prepare() noexcept;

			void bind() noexcept;
			void unbind() noexcept;

			[[nodiscard]] const unsigned int get_texture() const noexcept;

		private:
			unsigned int m_fbo;
			unsigned int m_depth_tex;
			int m_shadow_size;

			Shader m_shadowmap_pass;
		};
	} // namespace graphics
} // namespace galaxy

#endif