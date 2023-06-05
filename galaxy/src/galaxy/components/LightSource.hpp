///
/// LightSource.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_LIGHTSOURCE_HPP_
#define GALAXY_COMPONENTS_LIGHTSOURCE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Light.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// A LightSource in a scene.
		///
		class LightSource final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			LightSource();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			LightSource(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			LightSource(LightSource&&);

			///
			/// Move assignment operator.
			///
			LightSource& operator=(LightSource&&);

			///
			/// Destructor.
			///
			virtual ~LightSource();
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
			LightSource& operator=(const LightSource&) = delete;

			///
			/// Copy constructor.
			///
			LightSource(const LightSource&) = delete;

		public:
			///
			/// GPU compatible data structure.
			///
			graphics::Light m_light;
		};
	} // namespace components
} // namespace galaxy

#endif