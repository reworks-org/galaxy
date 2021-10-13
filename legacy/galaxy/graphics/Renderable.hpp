///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLE_HPP_
#define GALAXY_GRAPHICS_RENDERABLE_HPP_

#include <functional>

#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data to submit to a Render Layer.
		///
		struct Renderable final
		{
			///
			/// Vertex Array Object to bind.
			///
			unsigned int m_vao = 0;

			///
			/// Texture to bind.
			///
			unsigned int m_texture = 0;

			///
			/// Element buffer index count.
			///
			int m_index_count = 0;

			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int m_type = 0;

			///
			/// Function that binds a shader and sets uniform in prep for drawing.
			///
			std::function<void(void)> m_configure_shader;

			///
			/// Instance count. If greater than 0 object is drawn as an instance.
			///
			unsigned int m_instance_count = 0;
		};
	} // namespace graphics
} // namespace galaxy

#endif