///
/// CameraTag.hpp
/// starlight
///
/// Created by reworks on 28/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef REWORK_CAMERATAG_HPP_
#define REWORK_CAMERATAG_HPP_

#include "sl/math/Rect.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	class CameraTag final
	{
		friend class CameraSystem;
	public:
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
		/// Default constructor.
		/// Deleted.
		///
		CameraTag() = delete;

		///
		/// Update the camera position. Called internally by CameraSystem.
		/// We use the base type rather than the typedef to avoid includes.
		///
		void update(unsigned int playerEntity);

	public:
		Rect<float, int> m_bounds;
	};
}

#endif