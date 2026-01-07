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
		unsigned int target;

		///
		/// This is the glViewport. z,w is width/height.
		///
		glm::vec4 viewport;

		///
		/// OpenGL scissor region.
		/// https://gamedev.stackexchange.com/a/40713
		///
		glm::vec4 scissor;

		///
		/// Colour buffer clear colour.
		///
		glm::vec4 clear_col;

		///
		/// Depth buffer clear depth.
		///
		float clear_depth;
	};
} // namespace galaxy

#endif
