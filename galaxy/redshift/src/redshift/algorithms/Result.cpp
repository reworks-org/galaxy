///
/// Result.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Result.hpp"

namespace rs
{
	Result::Result() noexcept
		:m_boolean(false), m_pointOfCollision(0)
	{
	}

	Result::Result(bool boolean, std::uint32_t pointOfCollision) noexcept
		:m_boolean(boolean), m_pointOfCollision(pointOfCollision)
	{
	}
}