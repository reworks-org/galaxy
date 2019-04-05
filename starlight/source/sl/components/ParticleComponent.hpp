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

#include "sl/math/Vector2.hpp"
#include "sl/libs/sol2/forward.hpp"

namespace sl
{
	///
	/// Data about the particles an entity can emit.
	///
	class ParticleComponent final
	{
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		ParticleComponent();

		///
		/// \brief Constructor.
		///
		/// This component was designed to be created and destroyed on the fly, not sure why you would want it created in a lua script,
		/// but that option is avaliable.
		///
		/// \param table Sol::table containing data.
		///
		explicit ParticleComponent(const sol::table& table);

		///
		/// Constructor.
		///
		/// \param dx Direction of particle x.
		/// \param dy Direction of particle y.
		/// \param fade How much to reduce the alpha by per 1/60th a second.
		///
		ParticleComponent(const float dx, const float dy, const float fade);

		///
		/// Destructor.
		///
		~ParticleComponent() noexcept = default;

	public:
		///
		/// Rate the particles should fade at. 0.0 - 1.0.
		///
		float m_fade;

		///
		/// The direction the particles will travel in.
		///
		Vector2<float> m_direction;
	};
}

#endif