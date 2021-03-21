///
/// MainMenu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_MAINMENU_HPP_
#define SUPERCLUSTER_EDITOR_MAINMENU_HPP_

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Instance.hpp>

#include "instances/Editor.hpp"
#include "../Project.hpp"

using namespace galaxy;

namespace sc
{
	class MainMenu final : public core::Instance
	{
	public:
		MainMenu(core::Application* app, std::shared_ptr<Editor> editor, Project* project) noexcept;
		virtual ~MainMenu() noexcept;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		MainMenu() = delete;

	private:
		core::Application* m_app;
		std::shared_ptr<Editor> m_editor;
		Project* m_project;
	};
} // namespace sc

#endif