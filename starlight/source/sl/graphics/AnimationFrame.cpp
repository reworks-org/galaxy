///
/// AnimationFrame.cpp
/// starlight
///
/// Created by reworks on 07/03/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "AnimationFrame.hpp"

namespace sl
{
	AnimationFrame::AnimationFrame(std::uint32_t timePerFrame, const std::string& frameTextureID)
	:m_timePerFrame(timePerFrame), m_frameTextureID(frameTextureID)
	{
		// Argument constructor.
	}

	AnimationFrame::AnimationFrame(AnimationFrame&& af)
	:m_timePerFrame(af.m_timePerFrame), m_frameTextureID(af.m_frameTextureID)
	{
		// Explicit copy constructor.
	}

	AnimationFrame::AnimationFrame(const AnimationFrame& af)
	:m_timePerFrame(af.m_timePerFrame), m_frameTextureID(af.m_frameTextureID)
	{
		// Explicit move constructor.
	}

	AnimationFrame& AnimationFrame::operator=(const AnimationFrame &)
	{
		return *this;
	}
}