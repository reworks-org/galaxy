///
/// RenderData.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERDATA_HPP_
#define GALAXY_GRAPHICS_RENDERDATA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace galaxy
{
	///
	/// Uniform data passed to shader about an entity.
	///
	struct alignas(16) RenderData final
	{
		///
		/// Orthographic transform.
		///
		glm::mat4 transform;

		///
		/// Colour / tint.
		///
		glm::vec4 colour;

		///
		/// Entity ID.
		///
		int entity = -1;

		///
		/// Is this being rendered with GL_POINTS.
		///
		bool point = false;

		///
		/// Texture handle.
		///
		std::uint64_t handle = 0;
	};
} // namespace galaxy

#endif
