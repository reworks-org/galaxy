///
/// RigidBody.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RIGIDBODY_HPP_
#define GALAXY_COMPONENTS_RIGIDBODY_HPP_

#include "galaxy/fs/Serializable.hpp"
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

		public:
			///
			/// Body type.
			///
			physics::BodyType m_type;
		};
	} // namespace components
} // namespace galaxy

#endif