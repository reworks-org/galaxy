///
/// Point.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_POINT_HPP_
#define QUASAR_POINT_HPP_

#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Point graphic definition.
	///
	class Point final : public qs::VertexData
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
		/// \param colour Colour.
		///
		Point(const float x, const float y, const int size, protostar::Colour& colour) noexcept;

		///
		/// Destructor.
		///
		virtual ~Point() noexcept = default;

		///
		/// Create the point.
		///
		/// \param x X position.
		/// \param y Y position.
		/// \param size Size of the point.
		/// \param colour Colour.
		///
		void create(const float x, const float y, const int size, protostar::Colour& colour) noexcept;

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
	};
}

#endif