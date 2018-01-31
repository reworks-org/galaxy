///
/// ParticleEmitEvent.hpp
/// starlight
///
/// Created by reworks on 18/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARTICLEEMITEVENT_HPP_
#define STARLIGHT_PARTICLEEMITEVENT_HPP_

#include <vector>

#include "entt/core/hashed_string.hpp"

///
/// Please note, index [0] of particle count applies to index [0] of texture ids.
/// It's expected they are of the same length!
///
struct ParticleEmitEvent
{
	///
	/// Number of particles to emit for each type.
	///
	std::vector<unsigned int> m_particleCount;

	///
	/// ID's of the textures to use in the atlas.
	///
	std::vector<entt::HashedString> m_textureIDS;

	///
	/// X position to emit from.
	///
	float m_x;

	///
	/// Y position to emit from.
	///
	float m_y;

	///
	/// Direction max value.
	///
	float m_upper;

	///
	/// Direction min value.
	///
	float m_lower;

	///
	/// Layer to draw on.
	///
	int m_layer;

	///
	/// How much to reduce the alpha by per 1/60th a second.
	///
	float m_fade;
};

#endif