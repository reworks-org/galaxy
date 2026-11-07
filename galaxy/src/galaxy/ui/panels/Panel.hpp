///
/// Panel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_PANELS_PANEL_HPP_
#define GALAXY_UI_PANELS_PANEL_HPP_

namespace galaxy
{
	///
	/// Shared data for an imgui panel.
	///
	class Panel
	{
	public:
		///
		/// Constructor.
		///
		Panel() noexcept;

		///
		/// Move constructor.
		///
		Panel(Panel&&) noexcept;

		///
		/// Copy constructor.
		///
		Panel(const Panel&) noexcept;

		///
		/// Destructor.
		///
		virtual ~Panel() noexcept;

		///
		/// Draw the ImGui widgets.
		///
		virtual void draw() = 0;

		///
		/// Toggle visibility of panel.
		///
		void toggle() noexcept;

		///
		/// Check if panel is visible.
		///
		/// \return True if panel is visible.
		///
		[[nodiscard]]
		bool shown() const noexcept;

		///
		/// Move assignment operator.
		///
		Panel& operator=(Panel&&) noexcept;

		///
		/// Copy assignment operator.
		///
		Panel& operator=(const Panel&) noexcept;

	protected:
		///
		/// Should the panel be shown.
		///
		bool m_show;
	};
} // namespace galaxy

#endif
