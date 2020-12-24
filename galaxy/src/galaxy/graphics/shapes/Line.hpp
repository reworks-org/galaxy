///
/// Line.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LINE_HPP_
#define QUASAR_LINE_HPP_

#include <galaxy/graphics/Colour.hpp>

#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Line definition for renderer.
	///
	class Line final : public qs::VertexData, public qs::Transform
	{
	public:
		///
		/// Constructor.
		///
		Line() = default;

		///
		/// Constructor.
		///
		/// \param col Colour of line.
		/// \param x1 X1 position.
		/// \param y1 Y1 position.
		/// \param x2 X2 position.
		/// \param y2 Y2 position.
		///
		Line(pr::Colour& col, const float x1, const float y1, const float x2, const float y2);

		///
		/// Copy constructor.
		///
		Line(const Line&) = delete;

		///
		/// Move constructor.
		///
		Line(Line&&) = default;

		///
		/// Copy assignment operator.
		///
		Line& operator=(const Line&) = delete;

		///
		/// Move assignment operator.
		///
		Line& operator=(Line&&) = default;

		///
		/// Destructor.
		///
		virtual ~Line() = default;

		///
		/// Create the Line.
		///
		/// \param col Colour of line.
		/// \param x1 X1 position.
		/// \param y1 Y1 position.
		/// \param x2 X2 position.
		/// \param y2 Y2 position.
		/// \param thickness Sets the line thickness, defaulting to 1.
		///
		void create(pr::Colour& col, const float x1, const float y1, const float x2, const float y2);

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