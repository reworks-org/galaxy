///
/// CameraTag.hpp
/// starlight
///
/// Created by reworks on 28/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_CAMERATAG_HPP_
#define STARLIGHT_CAMERATAG_HPP_

#include "sl/math/Rect.hpp"
#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Represents a "camera" object in gamedev, usually player perspective.
	///
	class CameraTag final
	{
		friend class CameraSystem;
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Do NOT default construct this!
		/// Will throw an exception.
		/// Only here because entt requires it to deserialize if something goes wrong.
		///
		CameraTag();

		///
		/// Constructor.
		///
		/// \param table Table containing bounds.
		///
		CameraTag(const sol::table& table);

		///
		/// Constructor.
		///
		/// \param bounds Rect bounds of camera.
		///
		CameraTag(const Rect<float, int>& bounds);

		///
		/// Destructor.
		///
		virtual ~CameraTag() = default;

		///
		/// Default move assignment overload.
		///
		CameraTag& operator=(const CameraTag&);

	private:
		///
		/// Update the camera position. Called internally by CameraSystem.
		/// We use the base type rather than the typedef to avoid includes.
		///
		void update(unsigned int playerEntity);

		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_bounds);
		}

	public:
		///
		/// The dimensions of the camera. Does not have to be the same as the screen.
		///
		Rect<float, int> m_bounds;
	};
}

#endif