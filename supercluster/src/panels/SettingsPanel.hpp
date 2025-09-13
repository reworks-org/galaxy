///
/// SettingsPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_SETTINGSPANEL_HPP_
#define SUPERCLUSTER_PANELS_SETTINGSPANEL_HPP_

#include <span>

#include <ankerl/unordered_dense.h>
#include <nlohmann/json.hpp>

namespace sc
{
	class SettingsPanel final
	{
	public:
		SettingsPanel();
		~SettingsPanel();

		void                  load(const nlohmann::json& json);
		void                  render();
		const nlohmann::json& save();

	private:
		void do_object(nlohmann::json& json);
		void do_array(nlohmann::json& json);

	public:
		bool m_show = false;

	private:
		int            m_counter;
		nlohmann::json m_root;
	};
} // namespace sc

#endif
