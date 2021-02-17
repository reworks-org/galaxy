///
/// MouseWheel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_MOUSEWHEEL_HPP_
#define GALAXY_EVENTS_MOUSEWHEEL_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains mouse wheel movement data.
		///
		struct MouseWheel final
		{
			///
			/// Default constructor.
			///
			MouseWheel() noexcept;

			///
			/// Constructor.
			///
			/// \param x x offset. -1 or 1.
			/// \param y y offset. -1 or 1.
			///
			MouseWheel(const int x, const int y) noexcept;

			///
			/// Default destructor.
			///
			~MouseWheel() noexcept = default;

			///
			/// -1 or 1.
			///
			int m_x_offset;

			///
			/// -1 or 1.
			///
			int m_y_offset;
		};
	} // namespace events
} // namespace galaxy

#endif