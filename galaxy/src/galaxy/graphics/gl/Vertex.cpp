///
/// Vertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Vertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		std::vector<Vertex> gen_quad_vertices(const float width, const float height, float depth, const std::uint64_t handle) noexcept
		{
			depth = std::clamp(depth, -1.0f, 1.0f);

			// clang-format off
			const std::vector<Vertex> vertices = 
			{
				Vertex {
					.m_pos    = glm::vec3 {0.0f, 0.0f, depth}, 
					.m_texels = glm::vec2 {0.0f, 0.0f},
					.m_handle = handle
				},
				Vertex {
					.m_pos    = glm::vec3 {width, 0.0f, depth},
				    .m_texels = glm::vec2 {1.0f, 0.0f},
					.m_handle = handle
				},
				Vertex {
					.m_pos    = glm::vec3 {width, height, depth},
				    .m_texels = glm::vec2 {1.0f, 1.0f},
					.m_handle = handle
				},
				Vertex {
					.m_pos    = glm::vec3 {0.0f, height, depth},
				    .m_texels = glm::vec2 {0.0f, 1.0f},
					.m_handle = handle
				}
			};
			// clang-format on

			return vertices;
		}

		std::vector<unsigned int> gen_default_indices() noexcept
		{
			return {0u, 1u, 3u, 1u, 2u, 3u};
		}
	} // namespace graphics
} // namespace galaxy
