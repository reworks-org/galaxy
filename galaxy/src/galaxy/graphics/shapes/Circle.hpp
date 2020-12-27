///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <compare>

#include <glm/vec2.hpp>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Transform.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Circle definition for renderer.
		///
		class Circle final : public VertexData, public Transform
		{
		public:
			///
			/// Constructor.
			///
			Circle() = default;

			///
			/// Constructor.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			Circle(const float x, const float y, const float radius, const unsigned int fragments, graphics::Colour& colour);

			///
			/// Build from galaxy circle.
			///
			/// \param circle Protostar circle object.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			Circle(const pr::Circle& circle, const unsigned int fragments, graphics::Colour& colour);

			///
			/// Destructor.
			///
			virtual ~Circle() = default;

			///
			/// Create the Circle.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			void create(const float x, const float y, const float radius, const unsigned int fragments, graphics::Colour& colour);

			///
			/// Bind as active VA.
			///
			void bind();

			///
			/// Unbind as active VA.
			///
			void unbind();

			///
			/// Get x coord of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_x() const;

			///
			/// Get y coord of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_y() const;

			///
			/// Get xy as a vector.
			///
			/// \return Const reference to glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_xy() const;

			///
			/// Get radius of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float radius() const;

			///
			/// Strongly ordered spaceship operator.
			///
			auto operator<=>(const Circle&) const = default;

		private:
			///
			/// Position of circle.
			///
			glm::vec2 m_pos;

			///
			/// Radius of circle.
			///
			float m_radius;
		};
	} // namespace graphics
} // namespace galaxy

#endif