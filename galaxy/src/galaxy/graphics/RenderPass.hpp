///
/// RenderPass.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERPASS_HPP_
#define GALAXY_GRAPHICS_RENDERPASS_HPP_

#include <glm/vec4.hpp>

namespace galaxy
{
	///
	/// The configuration for drawing something.
	///
	struct RenderPass final
	{
		///
		/// This is the framebuffer to draw to.
		///
		unsigned int target = 0;

		///
		/// This is the glViewport. z,w is width/height.
		///
		glm::ivec4 viewport;

		///
		/// OpenGL scissor region.
		/// https://gamedev.stackexchange.com/a/40713
		///
		glm::ivec4 scissor;

		///
		/// Colour buffer clear colour.
		///
		glm::vec4 clear_col;

		///
		/// Depth buffer clear depth.
		///
		float clear_depth = 1.0f;
	};
} // namespace galaxy

#endif
