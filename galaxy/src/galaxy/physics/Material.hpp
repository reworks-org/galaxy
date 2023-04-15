///
/// Material.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_MATERIAL_HPP_
#define GALAXY_PHYSICS_MATERIAL_HPP_

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace physics
	{
		class Material final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Material() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Material(const nlohmann::json& json) noexcept;

			///
			/// Destructor.
			///
			~Material() noexcept;

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

		public:
			///
			/// How heavy it is in relation to its area.
			///
			float density;

			///
			/// How slippery it is.
			///
			float friction;

			///
			/// How bouncy the fixture is.
			///
			float restitution;

			///
			/// Restitution velocity threshold.
			///
			float restitution_threshold;
		};
	} // namespace physics
} // namespace galaxy

#endif