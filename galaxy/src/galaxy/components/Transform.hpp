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
			Transform() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Transform(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Transform(Transform&&) noexcept;

			///
			/// Move assignment operator.
			///
			Transform& operator=(Transform&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Transform() noexcept = default;

			///
			/// Translate (move) position.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			void translate(const float x, const float y) noexcept;

			///
			/// Rotate entity.
			///
			/// \param degrees Additive. Min 0, max 360.
			///
			void rotate(const float degrees) noexcept;

			///
			/// Set entity scale.
			///
			/// \param factor Scale multiplicative value. Must be positive.
			///
			void scale(const float factor) noexcept;

			///
			/// Sets position without moving the entity.
			///
			/// \param x X position to set.
			/// \param y Y position to set.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Set the entity rotation.
			///
			/// \param degrees Min 0, max 360.
			///
			void set_rotation(const float degrees) noexcept;

			///
			/// Set the rotational origin point.
			///
			/// \param x X position to set origin to.
			/// \param y Y position to set origin to.
			///
			void set_origin(const float x, const float y) noexcept;

			///
			/// Reset transform.
			///
			void reset() noexcept;

			///
			/// Get stored position.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_pos() noexcept;

			///
			/// Get stored rotation.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_rotation() const noexcept;

			///
			/// Get stored scale.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_scale() const noexcept;

			///
			/// Get origin point.
			///
			/// \return glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_origin() noexcept;

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return Reference to glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_transform();

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
			glm::vec3 m_scale;

			///
			/// Transform origin point.
			///
			glm::vec3 m_origin;
		};
	} // namespace components
} // namespace galaxy

#endif