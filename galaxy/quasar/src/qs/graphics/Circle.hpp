///
/// Circle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <protostar/graphics/Colour.hpp>

#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Circle definition for renderer.
	///
	class Circle final : public qs::VertexData
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
		/// \param thickness Thickness of circle.
		///
		Circle(const float x, const float y, const float radius, const int fragments, const pr::Colour& colour, const float thickness = 1.0f);

		///
		/// Destructor.
		///
		~Circle() noexcept = default;

		///
		/// Create the Circle.
		///
		/// \param x X position.
		/// \param y Y position.
		/// \param radius Radius of the circle.
		/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
		/// \param colour Colour.
		/// \param thickness Thickness of circle.
		///
		void create(const float x, const float y, const float radius, const int fragments, const pr::Colour& colour, const float thickness = 1.0f);

		///
		/// Bind as active VA.
		///
		void bind() noexcept;

		///
		/// Unbind as active VA.
		///
		void unbind() noexcept;

	private:
		///
		/// Thickness of circle.
		///
		float m_thickness;
	};
} // namespace qs

#endif