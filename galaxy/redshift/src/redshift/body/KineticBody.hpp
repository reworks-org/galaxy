///
/// KineticBody.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_KINETICBODY_HPP_
#define REDSHIFT_KINETICBODY_HPP_

#include "redshift/body/Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	class KineticBody final : public rs::Body
	{
	public:
		///
		/// Argument constructor.
		///
		KineticBody(const float density, const glm::vec2& size);

		///
		/// Argument constructor.
		///
		KineticBody(const float density, const float width, const float height);

		virtual ~KineticBody() = default;

		void apply_force(const float x_force, const float y_force);
		void apply_force(glm::vec2& force);

		void apply_horizontal_force(const float x_force);
		void apply_vertical_force(const float y_force);

		[[nodiscard]] const glm::vec2& get_force() const;

		const bool is_rigid() const noexcept override;

	private:
		KineticBody() = delete;

		///
		/// Force currently being applied to this object.
		///
		glm::vec2 m_force;
	};
} // namespace rs

#endif