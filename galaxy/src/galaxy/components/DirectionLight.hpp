///
/// DirectionLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_DIRECTIONLIGHT_HPP_
#define GALAXY_COMPONENTS_DIRECTIONLIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/light/Directional.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Directional light component.
		///
		class DirectionLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			DirectionLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			DirectionLight(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			DirectionLight(DirectionLight&&) noexcept;

			///
			/// Move assignment operator.
			///
			DirectionLight& operator=(DirectionLight&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~DirectionLight() noexcept = default;

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
			DirectionLight(const DirectionLight&) = delete;

			///
			/// Copy assignment operator.
			///
			DirectionLight& operator=(const DirectionLight&) = delete;

		public:
			///
			/// GLSL compatible object.
			///
			light::Directional m_light;
		};
	} // namespace components
} // namespace galaxy

#endif