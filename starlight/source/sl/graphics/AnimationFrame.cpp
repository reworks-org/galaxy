///
/// AnimationFrame.cpp
/// starlight
///
/// Created by reworks on 07/03/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/loguru/loguru.hpp"

#include "AnimationFrame.hpp"

namespace sl
{
	AnimationFrame::AnimationFrame()
	:m_timePerFrame(0), m_frameTextureID("error")
	{
	}

	AnimationFrame::AnimationFrame(std::uint32_t timePerFrame, const std::string& frameTextureID)
	:m_timePerFrame(timePerFrame), m_frameTextureID(frameTextureID)
	{
	}

	AnimationFrame::AnimationFrame(AnimationFrame&& af)
	:m_timePerFrame(af.m_timePerFrame), m_frameTextureID(af.m_frameTextureID)
	{
	}

	AnimationFrame::AnimationFrame(const AnimationFrame& af)
	:m_timePerFrame(af.m_timePerFrame), m_frameTextureID(af.m_frameTextureID)
	{
	}

	AnimationFrame& AnimationFrame::operator=(const AnimationFrame &)
	{
		return *this;
	}
}