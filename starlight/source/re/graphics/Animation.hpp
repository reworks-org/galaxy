///
///  Animation.hpp
///  starlight
///
///  Created by reworks on 31/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_ANIMATION_HPP_
#define REWORK_ANIMATION_HPP_

#include "sol2/sol_forward.hpp"

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
		/// Copy Constructor.
		///
		Animation(const Animation&) = default;

		///
		/// Move Constructor.
		///
		Animation(Animation&&) = default;

		///
		/// Destructor.
		///
		~Animation() = default;

	protected:
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

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

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Animation() = delete;
	};
}

#endif