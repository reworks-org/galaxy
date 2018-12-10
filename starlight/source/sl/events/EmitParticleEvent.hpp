///
/// EmitParticleEvent.hpp
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
	/// An event that contains information about particles to emit.
	///
	struct EmitParticleEvent final
	{
		///
		/// \brief Constructor.
		///
		/// Don't forget to add the particles to the map through m_particles.
		///
		inline EmitParticleEvent(float x, float y, float upper, float lower, int layer, float fade)
			:m_x(x), m_y(y), m_upper(upper), m_lower(lower), m_layer(layer), m_fade(fade)
		{
		}

		///
		/// \brief Information about particles to emit.
		/// 
		/// The string is the texture atlas id and the unsigned int is the amount of particles to emit for that texture.
		///
		std::vector<std::pair<std::string, unsigned int>> m_particles;

		///
		/// X position to emit from.
		///
		float m_x = 0.0f;

		///
		/// Y position to emit from.
		///
		float m_y = 0.0f;

		///
		/// Direction max value.
		///
		float m_upper = 0.0f;

		///
		/// Direction min value.
		///
		float m_lower = 0.0f;

		///
		/// Layer to draw on.
		///
		int m_layer = 0;

		///
		/// How much to reduce the alpha by per 1/60th a second. 
		/// Remember opacity is from 0.0f to 1.0f.
		///
		float m_fade = 0.0f;
	};
}

#endif