///
/// RenderCommand.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERCOMMAND_HPP_
#define GALAXY_GRAPHICS_RENDERCOMMAND_HPP_

#include "galaxy/graphics/RenderData.hpp"

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
			/// Layer to render on.
			///
			int layer = 0;

			///
			/// Shader uniform data.
			///
			RenderData uniforms;

			///
			/// OpenGL vertex array object.
			///
			unsigned int vao = 0;

			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int mode = 0;

			///
			/// OpenGL index (element) buffer count.
			///
			unsigned int count = 0;

			///
			/// Offset in vertex buffer marking beginning of element data.
			///
			void* offset = nullptr;

			///
			/// Number of instances to render.
			///
			int instances = 1;

			///
			/// Texture.
			///
			unsigned int texture = 0;
		};
	} // namespace graphics
} // namespace galaxy

#endif
