///
/// Result.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_RESULT_HPP_
#define REDSHIFT_RESULT_HPP_

#include <cstdint>

///
/// Core namespace.
///
namespace rs
{
	///
	/// Data structure holding results from a redshift algorithm call.
	///
	struct Result
	{
		///
		/// Default constructor.
		///
		Result() noexcept;

		///
		/// Argument constructor.
		///
		/// \param boolean Result for an algorithm.
		/// \param pointOfCollision For algorithms that also have a point of collision.
		///
		explicit Result(bool boolean, std::uint32_t pointOfCollision) noexcept;

		///
		/// Default destructor.
		///
		~Result() = default;

		///
		/// Result for an algorithm.
		///
		bool m_boolean;

		///
		/// For algorithms that also have a point of collision.
		///
		std::uint32_t m_pointOfCollision;
	};
}

#endif