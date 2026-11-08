///
/// ScenePanel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_PANELS_SCENEPANEL_HPP_
#define GALAXY_UI_PANELS_SCENEPANEL_HPP_

#include "galaxy/ui/panels/Panel.hpp"
#include "galaxy/ui/widgets/Popup.hpp"

namespace galaxy
{
	///
	/// Panel for editing scenes in engine.
	///
	class ScenePanel final : public Panel
	{
	public:
		///
		/// Constructor.
		///
		ScenePanel() noexcept;

		///
		/// Destructor.
		///
		virtual ~ScenePanel() noexcept;

		///
		/// Draw scene panel.
		///
		void draw() override;

	private:
		///
		/// For creating a new scene.
		///
		Popup m_new;

		///
		/// For removing all scenes.
		///
		Popup m_clear;
	};
} // namespace galaxy

/*
class ScenePanel final
	{
	public:
		ScenePanel();
		~ScenePanel();

		void render(scene::SceneManager& project, meta::vector<std::move_only_function<void(void)>>& tasks, Selected& selected);

	private:
		void draw_camera(scene::Scene* scene);
		void draw_physics(scene::Scene* scene);
		void draw_mapping(meta::vector<std::move_only_function<void(void)>>& tasks, scene::Scene* scene, Selected& selected);
		void draw_registry(Selected& selected, scene::Scene* scene, core::Registry& registry, ImGuiTextFilter& filter);

	public:
		bool m_show = true;

	private:

		bool                       m_prefab_popup = false;
		std::string                m_selected;
		ImGui::ComboAutoSelectData m_map_data;
		ImGui::ComboAutoSelectData m_prefab_data;
		ImGuiTextFilter            m_entity_filter;
		ImGuiTextFilter            m_map_entity_filter;
	};
*/

#endif
