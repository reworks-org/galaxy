///
/// MouseMoved.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEMOVED_HPP_
#define GALAXY_EVENTS_MOUSEMOVED_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains data relating to a mouse moved event.
		///
		struct MouseMoved final
		{
			///
			/// Default constructor.
			///
			MouseMoved();

			///
			/// Constructor.
			///
			/// \param x x-pos of event.
			/// \param y y-pos of event.
			///
			MouseMoved(const double x, const double y);

			///
			/// Default destructor.
			///
			~MouseMoved() = default;

			///
			/// Mouse x position.
			///
			double m_x;

			///
			/// Mouse y position.
			///
			double m_y;
		};
	} // namespace events
} // namespace galaxy

#endif