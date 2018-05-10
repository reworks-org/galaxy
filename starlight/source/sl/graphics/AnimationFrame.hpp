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

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/cereal/types/string.hpp"

namespace sl
{
	class AnimationFrame final
	{
		friend class cereal::access;

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

	private:
		///
		/// Cereal serialize function.
		/// 
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_timePerFrame, m_frameTextureID);
		}

	public:
		std::uint32_t m_timePerFrame;
		std::string m_frameTextureID;
	};
}

#endif