///
/// ScenePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_SCENEPANEL_HPP_
#define SUPERCLUSTER_PANELS_SCENEPANEL_HPP_

#include <imgui/combo_filter/imgui_combo_autoselect.h>

#include <galaxy/scene/SceneManager.hpp>

#include "../Selected.hpp"

using namespace galaxy;

namespace sc
{
	class ScenePanel final
	{
	  public:
		ScenePanel();
		~ScenePanel();

		void render(scene::SceneManager& project, meta::vector<std::function<void(void)>>& tasks, Selected& selected);

	  private:
		void draw_camera(scene::Scene* scene);
		void draw_physics(scene::Scene* scene);
		void draw_mapping(meta::vector<std::function<void(void)>>& tasks, scene::Scene* scene, Selected& selected);
		void draw_registry(Selected& selected, scene::Scene* scene, core::Registry& registry, ImGuiTextFilter& filter);

	  public:
		bool m_show = true;

	  private:
		bool                       m_show_new   = false;
		bool                       m_show_clear = false;
		std::string                m_selected;
		ImGui::ComboAutoSelectData m_map_data;
		ImGuiTextFilter            m_entity_filter;
		ImGuiTextFilter            m_map_entity_filter;
	};
} // namespace sc

#endif
