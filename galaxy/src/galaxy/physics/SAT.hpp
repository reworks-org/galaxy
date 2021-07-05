///
/// SAT.hpp
///
/// Based on code from ghost7:
/// https://github.com/ghost7/collision
///

#ifndef GALAXY_PHYSICS_SAT_HPP_
#define GALAXY_PHYSICS_SAT_HPP_

#include <vector>

#include <glm/vec2.hpp>

#include "galaxy/ecs/Entity.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace physics
	{
		///
		/// Class used to store data to resolve collisions using the
		/// separating axis theorem.
		///
		class SAT final
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param world World object to retrieve components from.
			/// \param entity Entity to get vertex data from.
			///
			SAT(core::World& world, const ecs::Entity entity);

			///
			/// Destructor.
			///
			~SAT() noexcept = default;

			///
			/// \brief Determine if this polygon intersects with another.
			///
			/// Also, output the
			/// minimum translation vector that will resolve the collision, if there is
			/// one.
			///
			/// \param objectb Other SAT object to test against.
			/// \param mtv Minimum transation vector used to resolve the collision.
			///
			/// \return True if the object intersects.
			///
			[[nodiscard]] const bool intersects(SAT& objectb, glm::vec2& mtv);

		private:
			///
			/// Helper class to contain vector projection information.
			///
			class Projection final
			{
			public:
				///
				/// Constructor.
				///
				Projection(float min, float max) noexcept;

				///
				/// Gets the magnitude of overlap of another projection. If the
				/// projections do not overlap, the magnitude will be 0.
				///
				/// \param other Projection to test against.
				///
				[[nodiscard]] const float get_overlap(const Projection& other);

			private:
				///
				/// Projection min.
				///
				float m_min;

				///
				/// Projection max.
				///
				float m_max;
			};

		private:
			///
			/// Default constructor.
			///
			SAT() = delete;

			///
			/// Project the polygon's vertices onto the specified axis.
			///
			/// \param axis Axis to test against.
			///
			[[nodiscard]] const Projection project(const glm::vec2& axis);

		private:
			///
			/// Vertexs to process.
			///
			std::vector<glm::vec2> m_vertexs;

			///
			/// Normals to process.
			///
			std::vector<glm::vec2> m_normals;
		};
	} // namespace physics
} // namespace galaxy

#endif