///
/// Animation.hpp
/// galaxy
///
/// Created by reworks on 31/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ANIMATION_HPP_
#define GALAXY_ANIMATION_HPP_

#include "galaxy/libs/sol2/forward.hpp"
#include "galaxy/graphics/AnimationFrame.hpp"
#include "galaxy/libs/cereal/types/vector.hpp"

namespace galaxy
{
	///
	/// Represents an animation. Can be constructed via a lua table or an argument constructor.
	///
	class Animation final
	{
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		Animation();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data. Frames MUST be in order.
		///
		explicit Animation(const sol::table& table);

		///
		/// Constructor.
		///
		/// \param isLooped Should this animation be looping?
		/// \param speed Speed to play the animation at, multiplier, so 1.0f is regular speed.
		/// \param totalFrames Total number of frames
		/// \param currentFrame Current frame index.
		/// \param frames A vector containing AnimationFrames.
		///
		Animation(const bool isLooped, const float speed, const unsigned int totalFrames, const unsigned int currentFrame, const std::vector<AnimationFrame>& frames);

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
		~Animation() noexcept = default;

	private:
		///
		/// Cereal serialize function.
		///
		template <class Archive>
		inline void serialize(Archive& ar)
		{
			ar(m_currentFrame, m_frames);
		}

	public:
		///
		/// Is the animation looping or not.
		///
		bool m_isLooped;

		///
		/// Speed of the animation.
		/// Multiplier, so 1.0f is regular speed.
		///
		float m_speed;

		///
		/// Total number of frames in animation.
		///
		unsigned int m_totalFrames;

		///
		/// The current frame the animation is on.
		///
		unsigned int m_currentFrame;

		///
		/// The frames that make up the animation.
		/// Each frame contains a duration and a textureID.
		///
		std::vector<AnimationFrame> m_frames;
	};
}

#endif