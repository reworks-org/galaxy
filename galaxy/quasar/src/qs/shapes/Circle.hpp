///
/// Circle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <protostar/graphics/Colour.hpp>

#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Circle definition for renderer.
	///
	class Circle final : public qs::VertexData, public qs::Transform
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
		Circle(const float x, const float y, const float radius, const unsigned int fragments, pr::Colour& colour);

		///
		/// Copy constructor.
		///
		Circle(const Circle&) = delete;

		///
		/// Move constructor.
		///
		Circle(Circle&&) = default;

		///
		/// Copy assignment operator.
		///
		Circle& operator=(const Circle&) = delete;

		///
		/// Move assignment operator.
		///
		Circle& operator=(Circle&&) = default;

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
		void create(const float x, const float y, const float radius, const unsigned int fragments, pr::Colour& colour);

		///
		/// Bind as active VA.
		///
		void bind();

		///
		/// Unbind as active VA.
		///
		void unbind();
	};
} // namespace qs

#endif