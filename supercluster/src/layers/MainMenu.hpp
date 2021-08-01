///
/// MainMenu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_MAINMENU_HPP_
#define SUPERCLUSTER_LAYERS_MAINMENU_HPP_

#include <galaxy/core/Application.hpp>

using namespace galaxy;

namespace sc
{
	class Editor;

	class MainMenu final : public core::Layer
	{
	public:
		MainMenu() noexcept;
		virtual ~MainMenu() noexcept;

		void events() override;
		void update() override;
		void pre_render() override;
		void render() override;

	public:
		std::shared_ptr<Editor> m_editor;
		core::Application* m_app;
	};
} // namespace sc

#endif