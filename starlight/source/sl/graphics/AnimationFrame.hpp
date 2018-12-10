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

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/cereal/types/string.hpp"

namespace sl
{
	///
	/// Represents a single frame of an animation.
	///
	class AnimationFrame final
	{
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains null data values.
		///
		AnimationFrame();

		///
		/// Constructor.
		///
		/// \param timePerFrame How long the animation should stay on this frame.
		/// \param frameTextureID The ID of the texture in the texture atlas.
		///
		AnimationFrame(const std::uint32_t timePerFrame, const std::string& frameTextureID);

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

		///
		/// Default destructor.
		///
		~AnimationFrame() noexcept = default;

	private:
		///
		/// Cereal serialize function.
		/// 
		template<class Archive>
		inline void serialize(Archive& ar)
		{
			ar(m_timePerFrame, m_frameTextureID);
		}

	public:
		///
		/// Time to spend on this frame.
		///
		std::uint32_t m_timePerFrame;

		///
		/// ID of the texture for this frame in the TextureAtlas.
		///
		std::string m_frameTextureID;
	};
}

#endif