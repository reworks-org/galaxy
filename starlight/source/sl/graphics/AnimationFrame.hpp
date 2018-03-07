///
/// AnimationFrame.hpp
/// starlight
///
/// Created by reworks on 07/03/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ANIMATIONFRAME_HPP_
#define STARLIGHT_ANIMATIONFRAME_HPP_

#include <string>
#include <cinttypes>

namespace sl
{
	class AnimationFrame
	{
	public:
		///
		/// \brief Default Constructor.
		///
		/// Please note this is only so a vector can be created with default initialized values.
		/// Not properly constructing the vector data will lead to issues.
		///
		AnimationFrame();

		///
		/// Constructor.
		///
		/// \param timePerFrame How long the animation should stay on this frame.
		/// \param frameTextureID The ID of the texture in the texture atlas.
		///
		AnimationFrame(std::uint32_t timePerFrame, const std::string& frameTextureID);

		///
		/// Copy Constructor.
		///
		AnimationFrame(AnimationFrame&& af);

		///
		/// Move Constructor.
		///
		AnimationFrame(const AnimationFrame& af);

		///
		/// Default move assignment overload.
		///
		AnimationFrame& operator=(const AnimationFrame&);

	public:
		std::uint32_t m_timePerFrame;
		std::string m_frameTextureID;
	};
}

#endif