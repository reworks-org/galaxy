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
	/// Panel for editing engine settings.
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
		///
		/// Handles rendering json objects.
		///
		/// \param json JSON object.
		///
		void do_object(nlohmann::json& json);

		///
		/// Handles rendering array objects.
		///
		/// \param json JSON array.
		///
		void do_array(nlohmann::json& json);

	private:
		///
		/// Internal level counter for rendering.
		///
		int m_counter;
	};
} // namespace galaxy

#endif
