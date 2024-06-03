///
/// Transform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TRANSFORM_HPP_
#define GALAXY_GRAPHICS_TRANSFORM_HPP_

#include <glm/mat4x4.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Defines the 2D transformation of an entity.
		///
		class Transform
		{
		  public:
			///
			/// Constructor.
			///
			Transform();

			///
			/// Move constructor.
			///
			Transform(Transform&&);

			///
			/// Move assignment operator.
			///
			Transform& operator=(Transform&&);

			///
			/// Copy constructor.
			///
			Transform(const Transform&);

			///
			/// Copy assignment operator.
			///
			Transform& operator=(const Transform&);

			///
			/// Destructor.
			///
			virtual ~Transform();

			///
			/// Translate (move) position.
			///
			/// \param x How far to translate on x axis.
			/// \param y How far to translate on y axis.
			///
			virtual void translate(const float x, const float y);

			///
			/// Rotate entity.
			///
			/// \param degrees Additive. Min -360, max 360.
			///
			virtual void rotate(const float degrees);

			///
			/// Set entity scale.
			///
			/// \param scale Value to scale by.
			///
			void scale(const float scale);

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
			/// \param degrees Min -360, max 360.
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
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_pos() const;

			///
			/// Get stored rotation.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_rotation() const;

			///
			/// Get stored scale.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_scale() const;

			///
			/// Get origin point.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& get_origin() const;

			///
			/// Retrieve internal transformation matrix.
			///
			/// \return glm::mat4 reference.
			///
			[[nodiscard]] glm::mat4& get_transform();

		  protected:
			///
			/// Cached for easy retrieval.
			/// Pos.
			///
			glm::vec2 m_pos;

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
			glm::vec2 m_origin;

			///
			/// Flag to see if transform needs to be recalculated.
			///
			bool m_dirty;

			///
			/// Combined transform.
			///
			glm::mat4 m_transform;
		};
	} // namespace graphics
} // namespace galaxy

#endif
