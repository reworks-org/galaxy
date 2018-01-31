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

#include "sol2/sol_forward.hpp"

namespace sl
{
	class Animation
	{
	public:
		///
		/// Default constructor.
		/// MSVC won't compile this class without a default constructor.
		/// Warning: This WILL throw an exception! IT IS NOT MEANT TO HAVE A DEFAULT CONSTRUCTOR!
		///
		Animation();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Animation(sol::table& table);

		///
		/// Destructor.
		///
		~Animation() = default;

	public:
		/// True / False
		bool m_isLooped;

		/// 1.0 is default... is a multiplier
		float m_speed;

		/// How long for each frame in milliseconds.
		std::int32_t m_timePerFrame;

		/// Total number of frames in animation.
		unsigned int m_totalFrames;

		/// current frame
		unsigned int m_currentFrame;

		/// Each frames name is a texture in the TextureAtlas.
		std::vector<std::string> m_frames;
	};
}

#endif