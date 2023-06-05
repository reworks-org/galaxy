///
/// Light.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_LIGHT_HPP_
#define GALAXY_COMPONENTS_LIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/LightData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// A lightsource in a scene.
		///
		class Light final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Light();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Light(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Light(Light&&);

			///
			/// Move assignment operator.
			///
			Light& operator=(Light&&);

			///
			/// Destructor.
			///
			virtual ~Light();
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
			/// Copy assignment operator.
			///
			Light& operator=(const Light&) = delete;

			///
			/// Copy constructor.
			///
			Light(const Light&) = delete;

		public:
			///
			/// GPU compatible data structure.
			///
			graphics::LightData m_data;
		};
	} // namespace components
} // namespace galaxy

#endif