///
/// EditorScene.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITOR_HPP_
#define SUPERCLUSTER_LAYERS_EDITOR_HPP_

#include <galaxy/core/Scene.hpp>
#include <galaxy/core/Window.hpp>

using namespace galaxy;

namespace sc
{
	class EditorScene final : public core::Scene
	{
	public:
		EditorScene();
		virtual ~EditorScene();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		core::Window* m_window;
	};
} // namespace sc

#endif