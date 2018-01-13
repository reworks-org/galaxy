///
///  CameraTag.hpp
///  starlight
///
///  Created by reworks on 28/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_CAMERA_HPP_
#define REWORK_CAMERA_HPP_

namespace sl
{
	class CameraTag
	{
	public:
		///
		/// Constructor.
		///
		/// \param e Entity for the camera to follow. Entity MUST have a transform component.
		///
		Camera(const sol::table);

		///
		/// Constructor.
		///
		/// \param bounds Bounds for the camera to follow.
		///
		Camera(Rect<float, int> bounds);

		///
		/// Constructor.
		///
		/// \param level 
		///
		void update(Level* level);

		///
		/// Return current camera bounds.
		///
		/// \return Rect<int> CONST
		///
		const Rect<float, int>& getBounds() const;

	private:
		entityx::Entity m_entityToFollow;
		Rect<float, int> m_bounds;
	};
}

#endif