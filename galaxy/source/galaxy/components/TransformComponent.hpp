///
/// TransformComponent.hpp
/// galaxy
///
/// Created by reworks on 30/09/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TRANSFORMCOMPONENT_HPP_
#define GALAXY_TRANSFORMCOMPONENT_HPP_

#include "galaxy/math/Rect.hpp"
#include "galaxy/libs/cereal/access.hpp"
#include "galaxy/libs/sol2/forward.hpp"

namespace galaxy
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
		/// Contains empty data values.
		///
		TransformComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		explicit TransformComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param layer Layer to render on.
		/// \param angle Angle to render at. In degrees.
		/// \param rect Position x, y and Dimensions width, height.
		///
		TransformComponent(const int layer, const float angle, const Rect<float, int>& rect);

		///
		/// Destructor.
		///
		~TransformComponent() noexcept = default;

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		inline void serialize(Archive& ar)
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
		/// In degrees.
		///
		float m_angle;

		///
		/// Dimensions of the entity.
		///
		Rect<float, int> m_rect;
	};
}

#endif