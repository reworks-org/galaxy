///
/// RenderCommand.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERCOMMAND_HPP_
#define GALAXY_GRAPHICS_RENDERCOMMAND_HPP_

#include "galaxy/graphics/Render2DUniform.hpp"
#include "galaxy/graphics/Renderable.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26495)
#endif

namespace galaxy
{
	namespace graphics
	{
		class Renderable;

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
			/// Renderable data;
			///
			Renderable* renderable;

			/*
			unsigned int m_vao;
			unsigned int m_texture;
			unsigned int m_index_count;
			int m_layer;
			*/
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif