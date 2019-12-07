///
/// CameraTag.hpp
/// galaxy
///
/// Created by reworks on 28/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CAMERATAG_HPP_
#define GALAXY_CAMERATAG_HPP_

#include "galaxy/math/Rect.hpp"
#include "galaxy/libs/cereal/access.hpp"
#include "galaxy/libs/sol2/forward.hpp"

galaxy
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
		/// Contains empty data values.
		///
		CameraTag();

		///
		/// Constructor.
		///
		/// \param table Table containing bounds.
		///
		explicit CameraTag(const sol::table& table);

		///
		/// Constructor.
		///
		/// \param bounds Rect bounds of camera.
		///
		explicit CameraTag(const Rect<float, int>& bounds);

		///
		/// Destructor.
		///
	    ~CameraTag() noexcept = default;

	private:
		///
		/// Update the camera position. Called internally by CameraSystem.
		/// We use the base type rather than the typedef to avoid includes.
		///
		void update(const unsigned int playerEntity);

		///
		/// Cereal serialize function.
		///
		template<class Archive>
		inline void serialize(Archive& ar)
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