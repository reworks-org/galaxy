///
/// Transform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TRANSFORM_HPP_
#define GALAXY_COMPONENTS_TRANSFORM_HPP_

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Defines the transformation of an entity.
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
			/// \brief Translate (move) position.
			///
			/// Does not set the position but moves it.
			/// By adding the parameters to the existing transformation matrix.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			void move(const float x, const float y) noexcept;

			///
			/// Rotate transformation in degrees.
			///
			/// \param degrees Max 360, min -360.
			///
			void rotate(const float degrees) noexcept;

			///
			/// Recalculates the model view matrix.
			///
			void recalculate();

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

			///
			/// Set the rotation point.
			///
			/// \param x X position to set origin to.
			/// \param y Y position to set origin to.
			///
			void set_rotation_origin(const float x, const float y) noexcept;

			///
			/// Get flag indicating if transform needs to be applied before rendering.
			///
			/// \return Const boolean.
			///
			[[nodiscard]] const bool is_dirty() const noexcept;

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return Reference to internal glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_transform();

			///
			/// Get stored rotation cache.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_rotation() const noexcept;

			///
			/// Get stored pos cache.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_pos() const noexcept;

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
			/// Rotation origin point.
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
			/// Cached for easiy retrieval.
			/// Rotation.
			///
			float m_rotate;

			///
			/// Cached for easiy retrieval.
			/// Pos.
			///
			glm::vec2 m_pos;

			///
			/// Combined transformation matrix.
			///
			glm::mat4 m_model;

		private:
			///
			/// Copy assignment operator.
			///
			Transform& operator=(const Transform&) = delete;

			///
			/// Copy constructor.
			///
			Transform(const Transform&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif