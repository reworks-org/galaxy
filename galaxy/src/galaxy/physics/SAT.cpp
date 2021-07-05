///
/// SAT.cpp
///
/// Based on code from ghost7:
/// https://github.com/ghost7/collision
///

#include <glm/geometric.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform2D.hpp"

#include "SAT.hpp"

namespace galaxy
{
	std::array<glm::vec2, 4> calc_transformed_pos(const float width, const float height, components::Transform2D* transform)
	{
		std::array<glm::vec2, 4> positions;

		auto result  = (transform->get_transform() * glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f});
		positions[0] = {result.x, result.y};

		result       = (transform->get_transform() * glm::vec4 {0.0f + width, 0.0f, 0.0f, 1.0f});
		positions[1] = {result.x, result.y};

		result       = (transform->get_transform() * glm::vec4 {0.0f + width, 0.0f + height, 0.0f, 1.0f});
		positions[2] = {result.x, result.y};

		result       = (transform->get_transform() * glm::vec4 {0.0f, 0.0f + height, 0.0f, 1.0f});
		positions[3] = {result.x, result.y};

		return positions;
	}

	namespace physics
	{
		SAT::Projection::Projection(float min, float max) noexcept
		    : m_min {min}, m_max {max}
		{
		}

		const float SAT::Projection::get_overlap(const Projection& other)
		{
			if (m_min > other.m_max || m_max < other.m_min)
			{
				return 0.0f;
			}
			else if (m_min < other.m_min)
			{
				return other.m_min - m_max;
			}
			else
			{
				return other.m_max - m_min;
			}
		}

		SAT::SAT(core::World& world, const ecs::Entity entity)
		{
			auto* renderable = world.get<components::Renderable>(entity);
			auto* transform  = world.get<components::Transform2D>(entity);

			if (renderable->m_type == graphics::Renderables::BATCHED)
			{
				auto* b2d = world.get<components::BatchSprite>(entity);

				const auto pos = calc_transformed_pos(b2d->get_region().m_width, b2d->get_region().m_height, transform);
				m_vertexs.assign(pos.begin(), pos.end());
			}
			else if (renderable->m_type == graphics::Renderables::SPRITE)
			{
				auto* s2d = world.get<components::Sprite>(entity);

				const auto pos = calc_transformed_pos(s2d->get_width(), s2d->get_height(), transform);
				m_vertexs.assign(pos.begin(), pos.end());
			}
			else
			{
				auto* p2d = world.get<components::Primitive2D>(entity);
				for (const auto& vertex : p2d->get_vertices())
				{
					const auto result = (transform->get_transform() * glm::vec4 {vertex.m_pos.x, vertex.m_pos.y, 0.0f, 1.0f});
					m_vertexs.emplace_back(result.x, result.y);
				}
			}

			// Calculate the polygon's normals. These are used as the axes to project
			// upon.
			const int size = static_cast<int>(m_vertexs.size());
			m_normals.resize(size);
			for (int i = 0; i < size; i++)
			{
				const int j    = (i + 1) % size;
				glm::vec2 axis = {(-m_vertexs[j].y + m_vertexs[i].y),
						  (m_vertexs[j].x - m_vertexs[i].x)};
				m_normals[i]   = glm::normalize(axis);
			}
		}

		const bool SAT::intersects(SAT& objectb, glm::vec2& mtv)
		{
			// Set the mtv to be the float max, so we can find a vector that is
			// smaller.
			float min_overlap  = FLT_MAX;
			glm::vec2 min_axis = {0.0f, 0.0f};

			// First test against this polygon's normals.
			for (const auto& axis : m_normals)
			{
				Projection p1       = project(axis);
				const Projection p2 = objectb.project(axis);
				const auto overlap  = p1.get_overlap(p2);

				if (overlap == 0)
				{
					mtv.x = 0.0f;
					mtv.y = 0.0f;
					return false;
				}
				else if (glm::abs(min_overlap) > glm::abs(overlap))
				{
					min_overlap = overlap;
					min_axis    = axis;
				}
			}

			// Now test against the other polygon's normals.
			for (const auto& axis : objectb.m_normals)
			{
				Projection p1       = project(axis);
				const Projection p2 = objectb.project(axis);
				const auto overlap  = p1.get_overlap(p2);
				if (overlap == 0)
				{
					mtv.x = 0.0f;
					mtv.y = 0.0f;
					return false;
				}

				if (glm::abs(min_overlap) > glm::abs(overlap))
				{
					min_overlap = overlap;
					min_axis    = axis;
				}
			}

			mtv = min_axis * min_overlap;
			return true;
		}

		const SAT::Projection SAT::project(const glm::vec2& axis)
		{
			auto min = glm::dot(axis, m_vertexs[0]);
			auto max = min;

			// Loop through all the vertices and project them onto the axis. Find the
			// minimum and maximum projection.
			for (const auto& vertex : m_vertexs)
			{
				const auto magnitude = glm::dot(axis, vertex);
				if (min > magnitude)
				{
					min = magnitude;
				}

				if (max < magnitude)
				{
					max = magnitude;
				}
			}

			return {min, max};
		}
	} // namespace physics
} // namespace galaxy