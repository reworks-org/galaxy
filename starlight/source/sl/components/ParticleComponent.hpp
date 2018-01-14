///
///  ParticleComponent.hpp
///  starlight
///
///  Created by reworks on 14/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARTICLECOMPONENT_HPP_
#define STARLIGHT_PARTICLECOMPONENT_HPP_

#include "entt/core/hashed_string.hpp"

namespace sl
{
	class ParticleComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param x XPOS.
		/// \param y YPOS.
		/// \param alpha Alpha value (opacity).
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(float x, float y, float alpha, entt::HashedString id);

		///
		/// Move Constructor.
		///
		ParticleComponent(ParticleComponent&&) = default;

		///
		/// Destructor.
		///
		~ParticleComponent() = default;

	public:
		float m_x;
		float m_y;
		float m_alpha;

		entt::HashedString m_atlasID;

	private:
		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

		///
		/// Default constructor.
		/// Deleted.
		///
		ParticleComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ParticleComponent(const ParticleComponent&) = delete;
	};
}

#endif