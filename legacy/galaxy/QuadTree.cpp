///
/// QuadTree.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "QuadTree.hpp"

namespace galaxy
{
	namespace math
	{
		Quadtree::Quadtree(const int level, const Rect<float>& bounds, int max_objects, int max_levels) noexcept
			: m_level {level}
			, m_bounds {bounds}
			, m_max_objects {max_objects}
			, m_max_levels {max_levels}
		{
			m_objects.reserve(m_max_objects);
		}

		Quadtree::~Quadtree() noexcept
		{
			clear();
		}

		void Quadtree::resize(const int width, const int height) noexcept
		{
			m_bounds.m_width  = width;
			m_bounds.m_height = height;
		}

		void Quadtree::insert(const Quadtree::Object& object)
		{
			if (m_nodes[0] != nullptr)
			{
				const auto index = get_index(object);
				if (index != -1)
				{
					m_nodes[index]->insert(object);
					return;
				}
			}

			m_objects.push_back(object);

			if (m_objects.size() > m_max_objects && m_level < m_max_levels)
			{
				if (m_nodes[0] == nullptr)
				{
					split();
				}

				for (auto it = m_objects.begin(); it != m_objects.end();)
				{
					const auto index = get_index(*it);
					if (index != -1)
					{
						m_nodes[index]->insert(*it);
						it = m_objects.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}

		void Quadtree::query(const Quadtree::Object& object, std::vector<Quadtree::Object*>& output)
		{
			const auto index = get_index(object);
			if (index != -1 && m_nodes[0] != nullptr)
			{
				m_nodes[index]->query(object, output);
			}

			output.reserve(output.size() + m_objects.size());
			for (auto& object : m_objects)
			{
				output.push_back(&object);
			}
		}

		void Quadtree::clear()
		{
			m_objects.clear();

			for (auto& node : m_nodes)
			{
				node.reset();
				node = nullptr;
			}
		}

		void Quadtree::split()
		{
			Rect<float> new_bounds;

			new_bounds.m_x      = m_bounds.m_x + (m_bounds.m_width / 2.0f);
			new_bounds.m_y      = m_bounds.m_y;
			new_bounds.m_width  = m_bounds.m_width / 2.0f;
			new_bounds.m_height = m_bounds.m_height / 2.0f;
			m_nodes[0]          = std::make_unique<Quadtree>(m_level + 1, new_bounds, m_max_objects, m_max_levels);

			new_bounds.m_x      = m_bounds.m_x;
			new_bounds.m_y      = m_bounds.m_y;
			new_bounds.m_width  = m_bounds.m_width / 2.0f;
			new_bounds.m_height = m_bounds.m_height / 2.0f;
			m_nodes[1]          = std::make_unique<Quadtree>(m_level + 1, new_bounds, m_max_objects, m_max_levels);

			new_bounds.m_x      = m_bounds.m_x;
			new_bounds.m_y      = m_bounds.m_y + (m_bounds.m_height / 2.0f);
			new_bounds.m_width  = m_bounds.m_width / 2.0f;
			new_bounds.m_height = m_bounds.m_height / 2.0f;
			m_nodes[2]          = std::make_unique<Quadtree>(m_level + 1, new_bounds, m_max_objects, m_max_levels);

			new_bounds.m_x      = m_bounds.m_x + (m_bounds.m_width / 2.0f);
			new_bounds.m_y      = m_bounds.m_y + (m_bounds.m_height / 2.0f);
			new_bounds.m_width  = m_bounds.m_width / 2.0f;
			new_bounds.m_height = m_bounds.m_height / 2.0f;
			m_nodes[3]          = std::make_unique<Quadtree>(m_level + 1, new_bounds, m_max_objects, m_max_levels);
		}

		const int Quadtree::get_index(const Quadtree::Object& object)
		{
			int index = -1;

			const double vert_midpoint = object.m_aabb->min().x + (object.m_aabb->size().x / 2.0f);
			const double hori_midpoint = object.m_aabb->min().y + (object.m_aabb->size().y / 2.0f);

			const bool fits_in_top    = (object.m_aabb->min().y < hori_midpoint && object.m_aabb->min().y + object.m_aabb->size().y < hori_midpoint);
			const bool fits_in_bottom = (object.m_aabb->min().y > hori_midpoint);

			if (object.m_aabb->min().x < vert_midpoint && object.m_aabb->min().x + object.m_aabb->size().x < vert_midpoint)
			{
				if (fits_in_top)
				{
					index = 1;
				}
				else if (fits_in_bottom)
				{
					index = 2;
				}
			}
			else if (object.m_aabb->min().x > vert_midpoint)
			{
				if (fits_in_top)
				{
					index = 0;
				}
				else if (fits_in_bottom)
				{
					index = 3;
				}
			}

			return index;
		}
	} // namespace math
} // namespace galaxy