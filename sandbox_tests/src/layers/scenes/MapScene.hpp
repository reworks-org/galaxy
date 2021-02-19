///
/// MapScene.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_MAPSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_MAPSCENE_HPP_

#include <galaxy/core/Scene.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/map/Map.hpp>

namespace sb
{
	class MapScene final : public galaxy::core::Scene
	{
	public:
		MapScene();
		virtual ~MapScene();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::core::Window* m_window;
	};
} // namespace sb

#endif