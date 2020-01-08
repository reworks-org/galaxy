///
/// Result.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_RESULT_HPP_
#define REDSHIFT_RESULT_HPP_

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
		bool m_success;
		std::uint32_t m_pointOfCollision;
	};
}

#endif