///
/// RenderCommand.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERCOMMAND_HPP_
#define GALAXY_GRAPHICS_RENDERCOMMAND_HPP_

#include "galaxy/graphics/Render2DUniform.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data to be passed to the renderer.
		///
		struct RenderCommand final
		{
			///
			/// Number of instances to render.
			///
			int instances;

			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int mode;

			///
			/// Render2D shader uniform data block.
			///
			Render2DUniform uniform_data;

			///
			/// OpenGL vertex array object.
			///
			unsigned int m_vao;

			///
			/// OpenGL texture handle.
			///
			unsigned int m_texture;

			///
			/// OpenGL index (element) buffer count.
			///
			unsigned int m_index_count;

			///
			/// Layer to render on.
			///
			int m_layer;

			///
			/// Is this a particle?
			///
			bool m_particle;
		};
	} // namespace graphics
} // namespace galaxy

#endif
