///
/// Vertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		std::array<Vertex, 4> gen_quad_vertices(const float width, const float height)
		{
			// clang-format off
			const std::array<graphics::Vertex, 4> vertices = 
			{
				graphics::Vertex {.m_pos =    glm::vec2 {0.0f, 0.0f}, 
								  .m_texels = glm::vec2 {0.0f, 0.0f}
				},
				graphics::Vertex {.m_pos =    glm::vec2 {width, 0.0f},
				                  .m_texels = glm::vec2 {1.0f, 0.0f}
				},
				graphics::Vertex {.m_pos =    glm::vec2 {width, height},
				                  .m_texels = glm::vec2 {1.0f, 1.0f}
				},
				graphics::Vertex {.m_pos =    glm::vec2 {0.0f, height},
				                  .m_texels = glm::vec2 {0.0f, 1.0f}
				}
			};
			// clang-format on

			return vertices;
		}

		std::array<unsigned int, 6> gen_default_indices()
		{
			return {0u, 1u, 3u, 1u, 2u, 3u};
		}

	} // namespace graphics
} // namespace galaxy
