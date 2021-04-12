///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_Point_HPP_
#define GALAXY_GRAPHICS_LIGHT_Point_HPP_

#include <glm/vec3.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace light
	{
		///
		/// Applies a point light to a scene.
		///
		class Point final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Point() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object holding data.
			///
			Point(const nlohmann::json& json);

			///
			/// Default destructor.
			///
			~Point() noexcept = default;

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
			/// Colour of the light.
			///
			glm::vec3 m_colour;

			///
			/// Position of the light.
			///
			glm::vec3 m_pos;
		};
	} // namespace light
} // namespace galaxy

#endif