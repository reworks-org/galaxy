///
/// Animation.hpp
/// starlight
///
/// Created by reworks on 31/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ANIMATION_HPP_
#define STARLIGHT_ANIMATION_HPP_

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"
#include "sl/graphics/AnimationFrame.hpp"

namespace sl
{
	class Animation final
	{
		friend class cereal::access;

	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Animation(sol::table& table);

		///
		/// Constructor.
		///
		/// \param isLooped Should this animation be looping?
		/// \param speed Speed to play the animation at, multiplier, so 1.0f is regular speed.
		/// \param totalFrames Total number of frames
		/// \param currentFrame Current frame index.
		/// \param frames A vector containing AnimationFrames.
		///
		Animation(bool isLooped, float speed, unsigned int totalFrames, unsigned int currentFrame, const std::vector<AnimationFrame>& frames);

		///
		/// Copy Constructor.
		///
		Animation(Animation&& animation);

		///
		/// Move Constructor.
		///
		Animation(const Animation& animation);

		///
		/// Destructor.
		///
		~Animation() = default;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Animation() = delete;

		///
		/// Cereal serialize function.
		///
		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_currentFrame, m_frames);
		}

	public:
		/// True / False
		bool m_isLooped;

		/// Multiplier, so 1.0f is regular speed.
		float m_speed;

		/// Total number of frames in animation.
		unsigned int m_totalFrames;

		/// current frame
		unsigned int m_currentFrame;

		/// Each frame contains a duration and a textureID.
		std::vector<AnimationFrame> m_frames;
	};
}

#endif