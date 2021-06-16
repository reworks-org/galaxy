///
/// Transform2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TRANSFORM2D_HPP_
#define GALAXY_COMPONENTS_TRANSFORM2D_HPP_

#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Defines the 2D transformation of an entity.
		///
		class Transform2D final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Transform2D() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Transform2D(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Transform2D(Transform2D&&) noexcept;

			///
			/// Move assignment operator.
			///
			Transform2D& operator=(Transform2D&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Transform2D() noexcept = default;

			///
			/// Sets position without moving the object.
			///
			/// \param x X position to set object to.
			/// \param y Y position to set object to.
			///
			void set_pos(const float x, const float y) noexcept;

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
			/// Rotate sprite.
			///
			/// \param degrees Max 360, min 0.
			///
			void rotate(const float degrees) noexcept;

			///
			/// Scale sprite.
			///
			/// \param factor 0.1 to 10.0.
			///
			void scale(const float factor) noexcept;

			///
			/// Set the origin point.
			///
			/// \param x X position to set origin to.
			/// \param y Y position to set origin to.
			///
			void set_origin(const float x, const float y) noexcept;

			///
			/// Recalculates the model view matrix.
			///
			void recalculate();

			///
			/// Get flag indicating if Transform2D needs to be applied before rendering.
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
			/// Get stored pos cache.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_pos() const noexcept;

			///
			/// Get stored rotation cache.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_rotation() const noexcept;

			///
			/// Get stored scale factor.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_scale() const noexcept;

			///
			/// Get origin.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_origin() const noexcept;

			///
			/// Reset Transform2D.
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
			/// Transform2D origin point.
			///
			glm::vec2 m_origin;

			///
			/// Translation matrix.
			///
			glm::mat4 m_translation;

			///
			/// Rotational matrix.
			///
			glm::mat4 m_rotation;

			///
			/// Scale matrix.
			///
			glm::mat4 m_scale;

			///
			/// Cached for easy retrieval.
			/// Pos.
			///
			glm::vec2 m_pos;

			///
			/// Cached for easy retrieval.
			/// Rotation.
			///
			float m_rotate;

			///
			/// Cached for easy retrieval.
			/// Scale.
			///
			float m_scale_factor;

			///
			/// Combined transformation matrix.
			///
			glm::mat4 m_model;

		private:
			///
			/// Copy assignment operator.
			///
			Transform2D& operator=(const Transform2D&) = delete;

			///
			/// Copy constructor.
			///
			Transform2D(const Transform2D&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif