///
/// EditorInstance.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_EDITORINSTANCE_HPP_
#define SUPERCLUSTER_INSTANCES_EDITORINSTANCE_HPP_

#include <galaxy/core/Instance.hpp>

#include "editor/Editor.hpp"
#include "editor/MainMenu.hpp"

using namespace galaxy;

namespace sc
{
	class EditorInstance final : public core::Instance
	{
	public:
		EditorInstance();
		virtual ~EditorInstance();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		void start();
		void end();

	private:
		bool m_game_mode = false;

		Editor m_editor;
		MainMenu m_menu;
		Project m_project;
	};
} // namespace sc

#endif