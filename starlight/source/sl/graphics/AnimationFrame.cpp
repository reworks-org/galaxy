///
/// AnimationFrame.cpp
/// starlight
///
/// Created by reworks on 07/03/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "AnimationFrame.hpp"

namespace sl
{
	AnimationFrame::AnimationFrame()
		:m_timePerFrame(0), m_frameTextureID(Locator::textureAtlas->m_nullTexture)
	{
	}

	AnimationFrame::AnimationFrame(const std::uint32_t timePerFrame, const std::string& frameTextureID)
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