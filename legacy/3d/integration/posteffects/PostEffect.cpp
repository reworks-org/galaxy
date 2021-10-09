///
/// PostEffect.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "PostEffect.hpp"

namespace galaxy
{
	namespace graphics
	{
		PostEffect::PostEffect()
		{
			m_fb.create(SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
		}
	} // namespace graphics
} // namespace galaxy