///
/// Theme.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Theme.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Theme::~Theme() noexcept
	{
		m_caches.clear();
	}
}