///
/// RigidBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RIGIDBODY_HPP_
#define GALAXY_COMPONENTS_RIGIDBODY_HPP_

#include <glm/mat4x4.hpp>

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/physics/AABB.hpp"
#include "galaxy/physics/BodyType.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Allows an entity to interact with other entities.
		///
		class RigidBody final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			RigidBody() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			RigidBody(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			RigidBody(RigidBody&&) noexcept;

			///
			/// Move assignment operator.
			///
			RigidBody& operator=(RigidBody&&) noexcept;

			///
			/// Destructor.
			///
			~RigidBody() noexcept = default;

			///
			/// Update the internal AABB box.
			///
			/// \param x New x position.
			/// \param y New y position.
			///
			/// \return Returns internal aabb after updating, for convenience.
			///
			[[maybe_unused]] const physics::AABB& update_aabb(const float x, const float y);

			///
			/// Set body type.
			///
			/// \param type New body type.
			///
			void set_bodytype(const physics::BodyType type) noexcept;

			///
			/// Set body size.
			///
			/// \param size New body size.
			///
			void set_size(const glm::vec2& size) noexcept;

			///
			/// Get AABB.
			///
			/// \return Const reference to body AABB.
			///
			[[nodiscard]] const physics::AABB& get_aabb() const noexcept;

			///
			/// Get rigid body type.
			///
			/// \return Const enum body type.
			///
			[[nodiscard]] const physics::BodyType get_type() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			RigidBody(const RigidBody&) = delete;

			///
			/// Copy assignment operator.
			///
			RigidBody& operator=(const RigidBody&) = delete;

		private:
			///
			/// Internal size.
			///
			glm::vec2 m_size;

			///
			/// AABB bounding box.
			///
			physics::AABB m_aabb;

			///
			/// Body type.
			///
			physics::BodyType m_type;
		};
	} // namespace components
} // namespace galaxy

#endif