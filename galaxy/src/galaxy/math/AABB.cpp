/*
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson: adapted and improved source code
 * from the AABBCC library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This codebase was mainly based on the AABBCC library, written by Lester
 * Hedges, which uses the Zlib license: https://github.com/lohedges/aabbcc.
 * Furthermore, the AABB tree implementation in the Simple Voxel Engine project
 * also influenced this library, which uses the MIT license:
 * https://github.com/JamesRandall/SimpleVoxelEngine.
 */

/*
	  GALAXY ENGINE EDIT:
	  THIS CODE FILE HAS BEEN EXTRACTED FROM abby.hpp TO AABB.hpp.
	  THIS CODEBASE HAS ALSO BEEN EXTENSIVELY MODIFIED FOR USE IN GALAXY.
  */

#include "galaxy/error/Log.hpp"

#include "AABB.hpp"

namespace galaxy
{
	namespace math
	{
		AABB::AABB() noexcept
		    : m_min {0.0f, 0.0f}, m_max {0.0f, 0.0f}, m_area {0.0}
		{
		}

		AABB::AABB(const glm::vec2& min, const glm::vec2& max) noexcept
		    : m_min {min}, m_max {max}, m_area {compute_area()}
		{
			if ((m_min.x > m_max.x) || (m_min.y > m_max.y))
			{
				GALAXY_LOG(GALAXY_ERROR, "AABB min is less than max.");
			}
		}

		void AABB::update_area() noexcept
		{
			m_area = compute_area();
		}

		void AABB::fatten(const std::optional<double>& factor) noexcept
		{
			if (!factor)
			{
				return;
			}

			const auto size = m_max - m_min;
			const auto dx   = *factor * size.x;
			const auto dy   = *factor * size.y;

			m_min.x -= dx;
			m_min.y -= dy;

			m_max.x += dx;
			m_max.y += dy;

			m_area = compute_area();
		}

		const bool AABB::contains(const AABB& other) const noexcept
		{
			for (auto i = 0; i < 2; ++i)
			{
				if (other.m_min[i] < m_min[i])
				{
					return false;
				}
				if (other.m_max[i] > m_max[i])
				{
					return false;
				}
			}
			return true;
		}

		const bool AABB::overlaps(const AABB& other, const bool touch_is_overlap) const noexcept
		{
			if (touch_is_overlap)
			{
				for (auto i = 0; i < 2; ++i)
				{
					if (other.m_max[i] < m_min[i] || other.m_min[i] > m_max[i])
					{
						return false;
					}
				}
			}
			else
			{
				for (auto i = 0; i < 2; ++i)
				{
					if (other.m_max[i] <= m_min[i] || other.m_min[i] >= m_max[i])
					{
						return false;
					}
				}
			}

			return true;
		}

		const double AABB::compute_area() const noexcept
		{
			// Sum of "area" of all the sides.
			auto sum = 0.0;

			// hold one dimension constant and multiply by all the other ones.
			for (auto d1 = 0; d1 < 2; ++d1)
			{
				auto product = 1.0; // "Area" of current side.

				for (auto d2 = 0; d2 < 2; ++d2)
				{
					if (d1 != d2)
					{
						const auto dx = m_max[d2] - m_min[d2];
						product *= dx;
					}
				}

				sum += product;
			}

			return 2.0 * sum;
		}

		const double AABB::area() const noexcept
		{
			return m_area;
		}

		glm::vec2 AABB::size() const noexcept
		{
			return m_max - m_min;
		}

		const glm::vec2& AABB::min() const noexcept
		{
			return m_min;
		}

		const glm::vec2& AABB::max() const noexcept
		{
			return m_max;
		}

		AABB AABB::merge(const AABB& fst, const AABB& snd) noexcept
		{
			glm::vec2 lower;
			glm::vec2 upper;

			for (auto i = 0; i < 2; ++i)
			{
				lower[i] = std::min(fst.m_min[i], snd.m_min[i]);
				upper[i] = std::max(fst.m_max[i], snd.m_max[i]);
			}

			return {lower, upper};
		}
	} // namespace math
} // namespace galaxy
