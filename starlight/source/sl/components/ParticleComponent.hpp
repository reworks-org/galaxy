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
		/// \param position x, y Position.
		/// \param direction x, y direction travelling.
		/// \param alpha Alpha value (opacity).
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(const Vector2<float>& position, const Vector2<float>& direction, float alpha, entt::HashedString id);

		///
		/// Constructor.
		///
		/// \param px x Position.
		/// \param py y Position.
		/// \param dx Direction of particle x.
		/// \param dy Direction of particle y.
		/// \param alpha Alpha value (opacity).
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(float px, float py, float dx, float dy, float alpha, entt::HashedString id);

		///
		/// Move Constructor.
		///
		ParticleComponent(ParticleComponent&&) = default;

		///
		/// Destructor.
		///
		~ParticleComponent() = default;

	public:
		Vector2<float> m_position;
		Vector2<float> m_direction;
		float m_alpha;

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