///
/// Transform3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TRANSFORM3D_HPP_
#define GALAXY_COMPONENTS_TRANSFORM3D_HPP_

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Defines the 3D transformation of an entity.
		///
		class Transform3D final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Transform3D() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Transform3D(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Transform3D(Transform3D&&) noexcept;

			///
			/// Move assignment operator.
			///
			Transform3D& operator=(Transform3D&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Transform3D() noexcept = default;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			/// \param z Z position to set object to.
			///
			void set_pos(const float x, const float y, const float z) noexcept;

			///
			/// \brief Translate (move) position.
			///
			/// Does not set the position but moves it.
			/// By adding the parameters to the existing Transform3Dation matrix.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			/// \param z How far to translate on z axis.
			///
			void move(const float x, const float y, const float z) noexcept;

			///
			/// Rotation of object.
			///
			/// \param degrees 0 - 360 degrees.
			///
			void rotate(const float degrees) noexcept;

			///
			/// Scale object.
			///
			/// \param factor 0.1 to 2.0.
			///
			void scale(const float factor) noexcept;

			///
			/// Set the origin point.
			///
			/// \param x X coord.
			/// \param y Y coord.
			/// \param z Z coord.
			///
			void set_origin(const float x, const float y, const float z) noexcept;

			///
			/// Set the rotation axis.
			///
			/// \param x X coord.
			/// \param y Y coord.
			/// \param z Z coord.
			///
			void set_rotation_axis(const float x, const float y, const float z) noexcept;

			///
			/// \brief Recalculates the model view matrix.
			///
			/// And the inverse matrix.
			///
			void recalculate();

			///
			/// Get flag indicating if Transform3D needs to be applied before rendering.
			///
			/// \return Const boolean.
			///
			[[nodiscard]] const bool is_dirty() const noexcept;

			///
			/// Retrieve transform (model) matrix.
			///
			/// \return Reference to glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_transform();

			///
			/// Retrieve inverse (model) matrix.
			///
			/// \return Reference to glm::mat3.
			///
			[[nodiscard]] const glm::mat3& get_inverse();

			///
			/// Get stored rotation cache.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_rotation() const noexcept;

			///
			/// Get stored pos cache.
			///
			/// \return Const glm::vec3 reference.
			///
			[[nodiscard]] const glm::vec3& get_pos() const noexcept;

			///
			/// Get stored scale factor.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_scale() const noexcept;

			///
			/// Get rotation axis.
			///
			/// \return Const glm::vec3 reference.
			///
			[[nodiscard]] const glm::vec3& get_rotation_axis() const noexcept;

			///
			/// Get origin.
			///
			/// \return Const glm::vec3 reference.
			///
			[[nodiscard]] const glm::vec3& get_origin() const noexcept;

			///
			/// Reset Transform3D.
			///
			void reset() noexcept;

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
			/// Update flag.
			///
			bool m_dirty;

			///
			/// Transform3D origin point.
			///
			glm::vec3 m_origin;

			///
			/// Rotational matrix.
			///
			glm::mat4 m_rotation;

			///
			/// Translation matrix.
			///
			glm::mat4 m_translation;

			///
			/// Scale matrix.
			///
			glm::mat4 m_scale;

			///
			/// Cached for easiy retrieval.
			/// Rotation.
			///
			float m_rotate;

			///
			/// Rotation axis.
			///
			glm::vec3 m_rotation_axis;

			///
			/// Cached for easiy retrieval.
			/// Pos.
			///
			glm::vec3 m_pos;

			///
			/// Cached for easiy retrieval.
			/// Scale factor.
			///
			float m_scale_factor;

			///
			/// Combined Transformation matrix.
			///
			glm::mat4 m_model;

			///
			/// Inverse Transformation matrix.
			///
			glm::mat3 m_inverse;

		private:
			///
			/// Copy assignment operator.
			///
			Transform3D& operator=(const Transform3D&) = delete;

			///
			/// Copy constructor.
			///
			Transform3D(const Transform3D&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif