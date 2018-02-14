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
#include "cereal/access.hpp"
#include "sol2/sol_forward.hpp"

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class TransformComponent final
	{
		friend class cereal::access;

	public:
		///
		/// Constructor.
		///
		/// \param entity Entity to attach to component fixtures.
		/// \param table sol::table containing data.
		///
		TransformComponent(entt::Entity entity, const sol::table& table);

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
		/// Default constructor.
		/// Deleted.
		///
		TransformComponent() = delete;

		///
		/// This method lets cereal know which data members to serialize.
		///
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(m_rect.x, m_rect.y, m_rect.width, m_rect.height, m_angle);
		}

	public:
		int m_layer;
		float m_angle;
		Rect<float, int> m_rect;
	};
}

#endif