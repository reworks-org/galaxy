///
/// PostProcessor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///
/// SMAA PORT: https://github.com/Asmodean-/SMAA-OpenGL
///

#ifndef GALAXY_GRAPHICS_POSTPROCESSOR_HPP_
#define GALAXY_GRAPHICS_POSTPROCESSOR_HPP_

#include <vector>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		class PostProcessor final
		{
		public:
			PostProcessor();
			~PostProcessor();

			void init(const int width, const int height);
			void resize(const int width, const int height);

			void bind() noexcept;
			void unbind() noexcept;

			void render();

		private:
			void init_smaa();

		private:
			int m_width;
			int m_height;

			RenderTexture m_fb;
			unsigned int m_screen_vbo;
			unsigned int m_screen_vao;

			Shader m_smaa_edge;
			Shader m_smaa_blend;
			Shader m_smaa_neighbourhood;

			unsigned int m_edge_tex;
			unsigned int m_blend_tex;
			unsigned int m_area_tex;
			unsigned int m_search_tex;

			unsigned int m_edge_fbo;
			unsigned int m_blend_fbo;
		};
	} // namespace graphics
} // namespace galaxy

#endif