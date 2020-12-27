///
/// StaticBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_BODY_STATICBODY_HPP_
#define GALAXY_PHYSICS_BODY_STATICBODY_HPP_

#include "galaxy/physics/body/Body.hpp"

namespace galaxy
{
	namespace physics
	{
		///
		/// A specialized body that cannot move.
		///
		class StaticBody final : public Body
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param density Density of this object.
			/// \param size Size of this object.
			///
			StaticBody(const float density, const glm::vec2& size);

			///
			/// Argument constructor.
			///
			/// \param density Density of this object.
			/// \param width Width of the object.
			/// \param height Height of the object.
			///
			StaticBody(const float density, const float width, const float height);

			///
			/// Virtual destructor.
			///
			virtual ~StaticBody() = default;

			///
			/// Get current velocity.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_vel() const override;

			///
			/// Check if this body is rigid or can move.
			///
			/// \return Const bool.
			///
			const bool is_rigid() const override;

		private:
			///
			/// Deleted default constructor.
			///
			StaticBody() = delete;
		};
	} // namespace physics
} // namespace galaxy

#endif