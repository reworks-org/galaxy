///
/// Transform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TRANSFORM_HPP_
#define GALAXY_COMPONENTS_TRANSFORM_HPP_

#include <glm/mat4x4.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Defines the 2D transformation of an entity.
		///
		class Transform final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			Transform();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Transform(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Transform(Transform&&);

			///
			/// Move assignment operator.
			///
			Transform& operator=(Transform&&);

			///
			/// Destructor.
			///
			virtual ~Transform() = default;

			///
			/// Translate (move) position.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			void translate(const float x, const float y);

			///
			/// Rotate entity.
			///
			/// \param degrees Additive. Min 0, max 360.
			///
			void rotate(const float degrees);

			///
			/// Set entity scale.
			///
			/// \param scale Value to scale by.
			///
			void set_scale(const float scale);

			///
			/// Set entity scale.
			///
			/// \param x Value to scale horizontal axis by.
			///
			void set_scale_horizontal(const float x);

			///
			/// Set entity scale.
			///
			/// \param y Value to scale vertical axis by.
			///
			void set_scale_vertical(const float y);

			///
			/// Sets position without moving the entity.
			///
			/// \param x X position to set.
			/// \param y Y position to set.
			///
			void set_pos(const float x, const float y);

			///
			/// Set the entity rotation.
			///
			/// \param degrees Min 0, max 360.
			///
			void set_rotation(const float degrees);

			///
			/// \brief Set the origin point.
			///
			/// Only affects rotation and scale.
			///
			/// \param x X position to set origin to.
			/// \param y Y position to set origin to.
			///
			void set_origin(const float x, const float y);

			///
			/// Reset transform.
			///
			void reset();

			///
			/// Get stored position.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_pos();

			///
			/// Get stored rotation.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_rotation() const;

			///
			/// Get stored scale.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_scale() const;

			///
			/// Get origin point.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_origin();

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return glm::mat4.
			///
			[[nodiscard]] glm::mat4 get_transform();

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
			Transform& operator=(const Transform&) = delete;

			///
			/// Copy constructor.
			///
			Transform(const Transform&) = delete;

		  private:
			///
			/// Cached for easy retrieval.
			/// Pos.
			///
			glm::vec3 m_pos;

			///
			/// Cached for easy retrieval.
			/// Rotation.
			///
			float m_rotation;

			///
			/// Cached for easy retrieval.
			/// Scale.
			///
			glm::vec2 m_scale;

			///
			/// Transform origin point.
			///
			glm::vec3 m_origin;
		};
	} // namespace components
} // namespace galaxy

#endif
