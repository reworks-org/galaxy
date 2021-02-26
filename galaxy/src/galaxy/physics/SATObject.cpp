///
/// SATObject.cpp
///
/// Based on code from ghost7:
/// https://github.com/ghost7/collision
///

#include "galaxy/core/World.hpp"
#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Ellipse.hpp"
#include "galaxy/components/Polygon.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/Transform.hpp"

#include "SATObject.hpp"

namespace galaxy
{
	namespace physics
	{
		SATObject::Projection::Projection(float min, float max) noexcept
		    : m_min {min}, m_max {max}
		{
		}

		const float SATObject::Projection::get_overlap(const Projection& other)
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

		SATObject::SATObject(core::World& world, const ecs::Entity entity)
		{
			auto* renderable = world.get<components::Renderable>(entity);
			if (renderable)
			{
				auto* transform = world.get<components::Transform>(entity);

				switch (renderable->m_type)
				{
					case graphics::Renderables::POINT:
					{
						GALAXY_LOG(GALAXY_WARNING, "Point is not supported yet for collision.");
					}
					break;

					case graphics::Renderables::LINE:
					{
						GALAXY_LOG(GALAXY_WARNING, "Line is not supported yet for collision.");
					}
					break;

					case graphics::Renderables::CIRCLE:
					{
						auto* circle = world.get<components::Circle>(entity);
						for (const auto& vertex : circle->get_vertexs())
						{
							const auto result = transform->get_transform(-1.0f, -1.0f) * glm::vec4(vertex.m_pos, 0.0f, 1.0f);
							m_vertexs.emplace_back(result.x, result.y);
						}
					}
					break;

					case graphics::Renderables::SPRITE:
					{
						auto* sprite = world.get<components::Sprite>(entity);
						for (const auto& vertex : sprite->get_vertexs())
						{
							const auto result = transform->get_transform(-1.0f, -1.0f) * glm::vec4(vertex.m_pos, 0.0f, 1.0f);
							m_vertexs.emplace_back(result.x, result.y);
						}
					}
					break;

					case graphics::Renderables::TEXT:
					{
						GALAXY_LOG(GALAXY_WARNING, "Text is not supported yet for collision.");
					}
					break;

					case graphics::Renderables::BATCHED:
					{
						auto* batch = world.get<components::BatchedSprite>(entity);
						for (const auto& vertex : batch->get_vertexs())
						{
							const auto result = transform->get_transform(-1.0f, -1.0f) * glm::vec4(vertex, 0.0f, 1.0f);
							m_vertexs.emplace_back(result.x, result.y);
						}
					}
					break;

					case graphics::Renderables::POLYGON:
					{
						auto* polygon = world.get<components::Polygon>(entity);
						for (const auto& vertex : polygon->get_vertexs())
						{
							const auto result = transform->get_transform(-1.0f, -1.0f) * glm::vec4(vertex.m_pos, 0.0f, 1.0f);
							m_vertexs.emplace_back(result.x, result.y);
						}
					}
					break;

					case graphics::Renderables::ELLIPSE:
					{
						auto* ellipse = world.get<components::Ellipse>(entity);
						for (const auto& vertex : ellipse->get_vertexs())
						{
							const auto result = transform->get_transform(-1.0f, -1.0f) * glm::vec4(vertex.m_pos, 0.0f, 1.0f);
							m_vertexs.emplace_back(result.x, result.y);
						}
					}
					break;
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
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to check collision with an entity that has no renderble component: {0}.", entity);
			}
		}

		const bool SATObject::intersects(SATObject& objectb, glm::vec2& mtv)
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
					mtv = glm::vec2(0, 0);
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

		const SATObject::Projection SATObject::project(const glm::vec2& axis)
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