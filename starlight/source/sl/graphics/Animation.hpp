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

#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	class Animation
	{
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
		/// \param timePerFrame how many milliseconds in a frame.
		/// \param totalFrames Total number of frames
		/// \param currentFrame Current frame index.
		/// \param frames A vector containing textureatlas ids of frames.
		///
		Animation(bool isLooped, float speed, std::uint32_t timePerFrame, unsigned int totalFrames, unsigned int currentFrame, const std::vector<std::string>& frames);

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

	public:
		/// True / False
		bool m_isLooped;

		/// Multiplier, so 1.0f is regular speed.
		float m_speed;

		/// How long for each frame in milliseconds.
		std::uint32_t m_timePerFrame;

		/// Total number of frames in animation.
		unsigned int m_totalFrames;

		/// current frame
		unsigned int m_currentFrame;

		/// Each frames name is a texture in the TextureAtlas.
		std::vector<std::string> m_frames;
	};
}

#endif