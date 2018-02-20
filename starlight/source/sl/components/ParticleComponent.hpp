///
/// ParticleComponent.hpp
/// starlight
///
/// Created by reworks on 14/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PARTICLECOMPONENT_HPP_
#define STARLIGHT_PARTICLECOMPONENT_HPP_

#include <string>

#include "sl/math/Vector2.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class ParticleComponent final
	{
	public:
		///
		/// \brief Constructor.
		///
		/// This component was designed to be created and destroyed on the fly, not sure why you would want it created in a lua script,
		/// but that option is avaliable.
		///
		/// \param entity Entity this component is attached to.
		/// \param table Sol::table containing data.
		///
		ParticleComponent(entt::Entity entity, const sol::table& table);

		///
		/// Constructor.
		///
		/// \param dx Direction of particle x.
		/// \param dy Direction of particle y.
		/// \param alpha Alpha value (opacity).
		/// \param fade How much to reduce the alpha by per 1/60th a second.
		/// \param id ID of particle texture in atlas.
		///
		ParticleComponent(float dx, float dy, float alpha, float fade, const std::string& id);

		///
		/// Destructor.
		///
		~ParticleComponent() = default;

		///
		/// Default move assignment overload.
		///
		ParticleComponent& operator=(const ParticleComponent&);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		ParticleComponent() = delete;

	public:
		float m_fade;
		float m_alpha;
		Vector2<float> m_direction;

		std::string m_id;
	};
}

#endif