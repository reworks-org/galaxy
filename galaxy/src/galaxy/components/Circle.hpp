///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <glm/vec2.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Circle definition for renderer.
		///
		class Circle final : public graphics::VertexData
		{
		public:
			///
			/// Constructor.
			///
			Circle() noexcept;

			///
			/// Constructor.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			Circle(const float x, const float y, const float radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Circle(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Circle(Circle&&) noexcept;

			///
			/// Move assignment operator.
			///
			Circle& operator=(Circle&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Circle() noexcept = default;

			///
			/// Create the Circle.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			void create(const float x, const float y, const float radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// Bind as active VA.
			///
			void bind() noexcept;

			///
			/// Unbind as active VA.
			///
			void unbind() noexcept;

			///
			/// Get x coord of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_x() const noexcept;

			///
			/// Get y coord of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_y() const noexcept;

			///
			/// Get xy as a vector.
			///
			/// \return Const reference to glm::vec2.
			///
			[[nodiscard]] const glm::vec2& get_xy() const noexcept;

			///
			/// Get radius of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float radius() const noexcept;

		private:
			///
			/// Position of circle.
			///
			glm::vec2 m_pos;

			///
			/// Radius of circle.
			///
			float m_radius;

		private:
			///
			/// Copy assignment operator.
			///
			Circle& operator=(const Circle&) = delete;

			///
			/// Copy constructor.
			///
			Circle(const Circle&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif