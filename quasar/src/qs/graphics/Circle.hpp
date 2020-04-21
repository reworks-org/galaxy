///
/// Circle.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Circle definition for renderer.
	///
	class Circle final
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
		///
		Circle(const float x, const float y, const float radius, const int fragments) noexcept;

		///
		/// Destructor.
		///
		~Circle() noexcept;

		///
		/// Create the Circle.
		///
		/// \param x X position.
		/// \param y Y position.
		/// \param radius Radius of the circle.
		/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
		///
		void create(const float x, const float y, const float radius, const int fragments) noexcept;

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
		void setThickness(const int thickness) noexcept;

		///
		/// Get total number of vertexs.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getCount() const noexcept;

	private:
		///
		/// Total number of vertexs.
		///
		unsigned int m_count;

		///
		/// Thickness of circle.
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