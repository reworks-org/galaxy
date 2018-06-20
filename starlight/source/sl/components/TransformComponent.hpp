///
/// TransformComponent.hpp
/// starlight
///
/// Created by reworks on 30/09/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TRANSFORMCOMPONENT_HPP_
#define STARLIGHT_TRANSFORMCOMPONENT_HPP_

#include "sl/math/Rect.hpp"
#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Allows for an entity to be transformed - x, y, z, etc.
	/// Required for rendering.
	///
	class TransformComponent final
	{
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Do NOT default construct this!
		/// Will throw an exception.
		/// Only here because entt requires it to deserialize if something goes wrong.
		///
		TransformComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		TransformComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param layer Layer to render on.
		/// \param angle Angle to render at.
		/// \param rect Position x, y and Dimensions width, height.
		///
		TransformComponent(int layer, float angle, const Rect<float, int>& rect);

		///
		/// Destructor.
		///
		~TransformComponent() = default;

		///
		/// Default move assignment overload.
		///
		TransformComponent& operator=(const TransformComponent&);

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_layer, m_angle, m_rect);
		}

	public:
		///
		/// Layer the entity is on.
		///
		int m_layer;

		///
		/// The angle the entity is rotated to.
		///
		float m_angle;

		///
		/// Dimensions of the entity.
		///
		Rect<float, int> m_rect;
	};
}

#endif