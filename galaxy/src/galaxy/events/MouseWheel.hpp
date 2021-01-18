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
			/// \param x x offset.
			/// \param y y offset.
			///
			MouseWheel(const double x, const double y) noexcept;

			///
			/// Default destructor.
			///
			~MouseWheel() noexcept = default;

			///
			/// X offset of scroll.
			///
			double m_x_offset;

			///
			/// Y offset of scroll.
			///
			double m_y_offset;
		};
	} // namespace events
} // namespace galaxy

#endif