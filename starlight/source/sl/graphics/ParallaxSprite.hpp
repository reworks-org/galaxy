///
/// ParallaxSprite.hpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// Copyright (c) 2018+ reworks.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARALLAXSPRITE_HPP_
#define STARLIGHT_PARALLAXSPRITE_HPP_

#include "sl/math/Vector2.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	class ParallaxSprite final
	{
	public:
		///
		/// Construct to parameters.
		///
		ParallaxSprite(float velX, float velY, float speed, entt::HashedString id);

		///
		/// Move Constructor.
		///
		ParallaxSprite(ParallaxSprite&&) = default;

		///
		/// Copy Constructor.
		///
		ParallaxSprite(const ParallaxSprite&) = default;
		
		///
		/// Cleans up world.
		///
		~ParallaxSprite() = default;

	public:
		float m_speed;
		Vector2<float> m_velocity;

		entt::HashedString m_atlasID;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		ParallaxSprite() = delete;
	};
}

#endif