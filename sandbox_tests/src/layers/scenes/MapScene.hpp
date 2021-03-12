///
/// MapScene.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_MAPSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_MAPSCENE_HPP_

#include <galaxy/scenes/Scene.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/map/Map.hpp>

namespace sb
{
	class MapScene final : public galaxy::scenes::Scene
	{
	public:
		MapScene(std::string_view name);
		virtual ~MapScene();

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

		[[nodiscard]] nlohmann::json sub_serialize() override;
		void sub_deserialize(const nlohmann::json& json) override;

	private:
		galaxy::core::Window* m_window;

		galaxy::map::Map m_map;
	};
} // namespace sb

#endif