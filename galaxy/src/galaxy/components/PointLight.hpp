///
/// PointLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POINTLIGHT_HPP_
#define GALAXY_COMPONENTS_POINTLIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/light/Point.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Point light component.
		///
		class PointLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			PointLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			PointLight(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			PointLight(PointLight&&) noexcept;

			///
			/// Move assignment operator.
			///
			PointLight& operator=(PointLight&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~PointLight() noexcept = default;

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
			PointLight(const PointLight&) = delete;

			///
			/// Copy assignment operator.
			///
			PointLight& operator=(const PointLight&) = delete;

		public:
			///
			/// GLSL compatible object.
			///
			light::Point m_light;
		};
	} // namespace components
} // namespace galaxy

#endif