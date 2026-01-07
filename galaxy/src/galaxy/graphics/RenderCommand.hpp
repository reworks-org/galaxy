///
/// RenderCommand.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERCOMMAND_HPP_
#define GALAXY_GRAPHICS_RENDERCOMMAND_HPP_

#include "galaxy/graphics/gl/GLEnums.hpp"
#include "galaxy/graphics/gl/ShaderBuffer.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"
#include "galaxy/graphics/RenderData.hpp"
#include "galaxy/utils/Memory.hpp"

namespace galaxy
{
	///
	/// Data required to draw an object.
	///
	struct RenderCmd final
	{
		///
		/// OpenGL rendering mode/type.
		/// Required.
		///
		GLRenderMode type;

		///
		/// Vertices if type is quad for batch rendering.
		///
		std::vector<Vertex> vertices;

		///
		/// Vertex array object if type is not quad.
		/// Not used by batch renderer.
		///
		VertexArray* vao;

		///
		/// Uniform data for this command.
		///
		RenderData data;
	};
} // namespace galaxy

#endif
