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
#include "sol2/sol_forward.hpp"

namespace sl
{
	class CameraTag final
	{
	public:
		///
		/// Constructor.
		///
		/// \param e Entity for the camera to follow. Entity MUST have a transform component.
		///
		CameraTag(const sol::table& table);

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

	public:
		Rect<float, int> m_bounds;
	};
}

#endif