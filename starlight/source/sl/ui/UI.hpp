///
/// UI.hpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UI_HPP_
#define STARLIGHT_UI_HPP_

#include <vector>

#include "sl/ui/Panel.hpp"

namespace sl
{
	///
	/// Manages UI for a dynamic array of Panels.
	/// Stores panels as unique_ptrs.
	///
	class UI final
	{
	public:
		///
		/// Constructor.
		///
		UI() = default;

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
		void update();

		///
		/// Render UI to screen.
		///
		void render();

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