///
/// Line.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LINE_HPP_
#define QUASAR_LINE_HPP_

#include <protostar/graphics/Colour.hpp>

#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Line definition for renderer.
	///
	class Line final : public qs::VertexData
	{
	public:
		///
		/// Constructor.
		///
		Line() noexcept;

		///
		/// Constructor.
		///
		/// \param col Colour of line.
		/// \param x1 X1 position.
		/// \param y1 Y1 position.
		/// \param x2 X2 position.
		/// \param y2 Y2 position.
		/// \param thickness Sets the line thickness, defaulting to 1.
		///
		Line(const pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const pr::positive_float auto thickness = 1.0f);

		///
		/// Destructor.
		///
		~Line() noexcept = default;

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
		void create(const pr::Colour& col, const float x1, const float y1, const float x2, const float y2, const pr::positive_float auto thickness = 1.0f);

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
		/// Line thickness.
		///
		float m_thickness;
	};
} // namespace qs

#endif