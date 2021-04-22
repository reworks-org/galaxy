///
/// SpotLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPOTLIGHT_HPP_
#define GALAXY_COMPONENTS_SPOTLIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/light/Spot.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Spot light component.
		///
		class SpotLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			SpotLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			SpotLight(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			SpotLight(SpotLight&&) noexcept;

			///
			/// Move assignment operator.
			///
			SpotLight& operator=(SpotLight&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~SpotLight() noexcept = default;

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
			SpotLight(const SpotLight&) = delete;

			///
			/// Copy assignment operator.
			///
			SpotLight& operator=(const SpotLight&) = delete;

		public:
			///
			/// GLSL compatible object.
			///
			light::Spot m_light;
		};
	} // namespace components
} // namespace galaxy

#endif