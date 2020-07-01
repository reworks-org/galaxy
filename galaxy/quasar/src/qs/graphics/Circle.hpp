///
/// Circle.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <protostar/system/Colour.hpp>

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
		///
		Circle(const float x, const float y, const float radius, const int fragments, protostar::Colour& colour) noexcept;

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
		///
		void create(const float x, const float y, const float radius, const int fragments, protostar::Colour& colour) noexcept;

		///
		/// Bind as active VA.
		///
		void bind() noexcept;

		///
		/// Unbind as active VA.
		///
		void unbind() noexcept;

		///
		/// Set thickness of circle line.
		///
		/// \param thickness Thickness.
		///
		void setThickness(const float thickness) noexcept;

	private:
		///
		/// Thickness of circle.
		///
		float m_thickness;
	};
}

#endif