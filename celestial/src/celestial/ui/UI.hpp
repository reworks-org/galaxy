///
/// UI.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UI_HPP_
#define CELESTIAL_UI_HPP_

#include <vector>

#include "celestial/ui/Panel.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Manages UI for a dynamic array of Panels.
	/// Stores panels as unique_ptrs.
	///
	class UI final
	{
	public:
		///
		/// Default constructor.
		///
		UI() noexcept = default;

		///
		/// Destructor.
		///
		~UI() noexcept;

		///
		/// Add a Panel to the UI.
		///
		/// \param args Arguments for panel to construct.
		///
		/// \return Returns pointer to newly created panel.
		///
		template<typename... Args>
		Panel* addPanel(Args&&... args);

		///
		/// Update the UI.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt);

		///
		/// Render UI to screen.
		///
		/// \param renderer Renderer to use when drawing. Needs to be consistent.
		///
		void render(celestial::Renderer* renderer);

	private:
		///
		/// Collection of panels in the ui.
		///
		std::vector<std::unique_ptr<Panel>> m_panels;
	};

	template<typename... Args>
	inline Panel* UI::addPanel(Args&&... args)
	{
		// Forward arguments to std::vector's construct in place method.
		Panel* ref = m_panels.emplace_back(std::make_unique<Panel>(std::forward<Args>(args)...)).get();
		
		// Then return a pointer to object placed.
		return ref;
	}
}

#endif