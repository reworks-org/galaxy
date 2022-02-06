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

#ifndef GALAXY_MATH_AABB_HPP_
#define GALAXY_MATH_AABB_HPP_

#include <optional>

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace math
	{
		///
		/// Represents an AABB (Axis-Aligned Bounding Box).
		///
		class AABB final
		{
		public:
			///
			/// Constructor.
			///
			AABB() noexcept;

			///
			/// Creates an AABB.
			///
			/// \param min the lower bounds of the AABB.
			/// \param max the upper bounds of the AABB.
			///
			AABB(const glm::vec2& min, const glm::vec2& max) noexcept;

			///
			/// Set AABB min and max.
			///
			/// \param min the lower bounds of the AABB.
			/// \param max the upper bounds of the AABB.
			///
			void set(const glm::vec2& min, const glm::vec2& max) noexcept;

			///
			/// Updates the stored area.
			///
			void update_area() noexcept;

			///
			/// \brief Fattens the AABB by increasing its size.
			///
			/// This function has no effect if the supplied value is `std::nullopt`.
			///
			/// \param factor Fattening factor.
			///
			void fatten(const std::optional<float>& factor) noexcept;

			///
			/// Indicates whether or not the supplied AABB is contained within the invoked AABB.
			///
			/// The supplied AABB is still considered to be contained within the invoked AABB
			/// if the borders of the inner AABB are overlapping the borders of the outer AABB.
			///
			/// \param other the AABB to check.
			///
			/// \return True if the supplied AABB is contained in the AABB; `false` otherwise.
			///
			[[nodiscard]] const bool contains(const AABB& other) const noexcept;

			///
			/// Indicates whether or not two AABBs are overlapping each other.
			///
			/// \param other the other AABB to compare with.
			/// \param touch_is_overlap `true` if the AABBs are considered to be overlapping if they touch; `false` otherwise.
			///
			/// \return True if the two AABBs are overlapping each other; `false` otherwise.
			///
			[[nodiscard]] const bool overlaps(const AABB& other, const bool touch_is_overlap) const noexcept;

			///
			/// Computes and returns the area of the AABB.
			///
			/// \return the computed area of the AABB.
			///
			[[nodiscard]] const float compute_area() const noexcept;

			///
			/// Returns the stored area of the AABB.
			///
			/// \return the stored area of the AABB.
			///
			[[nodiscard]] const float area() const noexcept;

			///
			/// Returns the size of the AABB.
			///
			/// \return the size of the AABB (width and height).
			///
			[[nodiscard]] glm::vec2 size() const noexcept;

			///
			/// Returns the min of the AABB.
			///
			/// \return The min vector.
			///
			[[nodiscard]] const glm::vec2& min() const noexcept;

			///
			/// Returns the max of the AABB.
			///
			/// \return The max vector.
			///
			[[nodiscard]] const glm::vec2& max() const noexcept;

			///
			/// Returns an AABB that is the union of the supplied pair of AABBs.
			///
			/// \param fst the first AABB.
			/// \param snd the second AABB.
			///
			/// \return an AABB that is the union of the two supplied AABBs.
			///
			[[nodiscard]] static AABB merge(const AABB& fst, const AABB& snd) noexcept;

		private:
			///
			/// AABB min.
			///
			glm::vec2 m_min;

			///
			/// AABB max.
			///
			glm::vec2 m_max;

			///
			/// AABB area.
			///
			float m_area;
		};

		[[nodiscard]] inline const bool operator==(const AABB& lhs, const AABB& rhs) noexcept
		{
			return (lhs.min() == rhs.min()) && (lhs.max() == rhs.max());
		}

		[[nodiscard]] inline const bool operator!=(const AABB& lhs, const AABB& rhs) noexcept
		{
			return !(lhs == rhs);
		}
	} // namespace math
} // namespace galaxy

#endif