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

#include "sl/math/Vector2.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	class ParticleComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param direction x, y direction travelling.
		/// \param alpha Alpha value (opacity).
		/// \param fade How much to reduce the alpha by per 1/60th a second.
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(const Vector2<float>& direction, float alpha, float fade, entt::HashedString id);

		///
		/// Constructor.
		///
		/// \param dx Direction of particle x.
		/// \param dy Direction of particle y.
		/// \param alpha Alpha value (opacity).
		/// \param fade How much to reduce the alpha by per 1/60th a second.
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(float dx, float dy, float alpha, float fade, entt::HashedString id);

		///
		/// Move Constructor.
		///
		ParticleComponent(ParticleComponent&&) = default;

		///
		/// Destructor.
		///
		~ParticleComponent() = default;

	public:
		float m_fade;
		float m_alpha;
		Vector2<float> m_direction;

		entt::HashedString m_id;

	private:
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