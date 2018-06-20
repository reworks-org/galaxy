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

namespace sl
{
	///
	/// \brief An event that contains information about particles to emit.
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
		std::vector<std::string> m_textureIDS;

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
		/// Remember opacity is from 0.0f to 1.0f.
		///
		float m_fade;
	};
}

#endif