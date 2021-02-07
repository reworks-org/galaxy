///
/// EditorLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITORLAYER_HPP_
#define SUPERCLUSTER_LAYERS_EDITORLAYER_HPP_

#include <galaxy/core/Layer.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Camera.hpp>

#include "scenes/EditorScene.hpp"

using namespace galaxy;

namespace sc
{
	class EditorLayer final : public core::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::core::Window* m_window;
		std::unique_ptr<EditorScene> m_editor_scene;
	};
} // namespace sc

#endif