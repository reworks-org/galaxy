///
/// Point.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_POINT_HPP_
#define QUASAR_POINT_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Point graphic definition.
	///
	class Point final
	{
	public:
		///
		/// Constructor.
		///
		Point() noexcept;

		///
		/// Constructor.
		///
		/// \param x X position.
		/// \param y Y position.
		/// \param size Size of the point.
		///
		Point(const float x, const float y, const int size) noexcept;

		///
		/// Destructor.
		///
		~Point() noexcept;

		///
		/// Create the point.
		///
		/// \param x X position.
		/// \param y Y position.
		/// \param size Size of the point.
		///
		void create(const float x, const float y, const int size) noexcept;

		///
		/// Bind as active VA.
		///
		void bind() noexcept;

		///
		/// Unbind as active VA.
		///
		void unbind() noexcept;

		///
		/// Get size of point.
		///
		/// \return Const integer.
		///
		const int getSize() const noexcept;

	private:
		///
		/// Size of point.
		///
		int m_size;

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