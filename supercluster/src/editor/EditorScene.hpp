///
/// EditorScene.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_EDITORSCENE_HPP_
#define SUPERCLUSTER_EDITOR_EDITORSCENE_HPP_

#include <galaxy/scenes/Scene.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/systems/CollisionSystem.hpp>

using namespace galaxy;

namespace sc
{
	class EditorScene final : public scenes::Scene
	{
	public:
		EditorScene(std::string_view name);
		virtual ~EditorScene();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

		[[nodiscard]] systems::CollisionSystem* get_collision_system() noexcept;

		[[nodiscard]] nlohmann::json sub_serialize() override;
		void sub_deserialize(const nlohmann::json& json) override;

	private:
		systems::CollisionSystem* m_collision_system;
		core::Window* m_window;
	};
} // namespace sc

#endif