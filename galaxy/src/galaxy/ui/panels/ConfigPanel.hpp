///
/// ConfigPanel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_PANELS_CONFIGPANEL_HPP_
#define GALAXY_UI_PANELS_CONFIGPANEL_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/ui/panels/Panel.hpp"

namespace galaxy
{
	///
	/// Shared data for an imgui panel.
	///
	class ConfigPanel final : public Panel
	{
	public:
		///
		/// Constructor.
		///
		ConfigPanel() noexcept;

		///
		/// Destructor.
		///
		virtual ~ConfigPanel() noexcept;

		///
		/// Draw config panel.
		///
		void draw() override;

	private:
		void do_object(nlohmann::json& json);
		void do_array(nlohmann::json& json);

	private:
		int m_counter;
	};
} // namespace galaxy

#endif
