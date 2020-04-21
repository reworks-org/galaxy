///
/// Line.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LINE_HPP_
#define QUASAR_LINE_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Line definition for renderer.
	///
	class Line final
	{
	public:
		///
		/// Constructor.
		///
		Line() noexcept;

		///
		/// Constructor.
		///
		/// \param x1 X1 position.
		/// \param y1 Y1 position.
		/// \param x2 X2 position.
		/// \param y2 Y2 position.
		/// \param thickness Sets the line thickness, defaulting to 1.
		///
		Line(const float x1, const float y1, const float x2, const float y2, const int thickness = 1) noexcept;

		///
		/// Destructor.
		///
		~Line() noexcept;

		///
		/// Create the Line.
		///
		/// \param x1 X1 position.
		/// \param y1 Y1 position.
		/// \param x2 X2 position.
		/// \param y2 Y2 position.
		/// \param thickness Sets the line thickness, defaulting to 1.
		///
		void create(const float x1, const float y1, const float x2, const float y2, const int thickness = 1) noexcept;

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
		int m_thickness;

		///
		/// VertexArray handle.
		///
		unsigned int m_va;

		///
		/// VertexBuffer handle.
		///
		unsigned int m_vb;
	};
}

#endif