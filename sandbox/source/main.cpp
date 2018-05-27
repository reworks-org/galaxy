///
/// main.cpp
/// sandbox
///
/// Created by reworks on 08/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <memory>
#include <sl/tags/CameraTag.hpp>
#include <sl/core/Application.hpp>
#include <sl/systems/RenderSystem.hpp>
#include <sl/systems/CameraSystem.hpp>
#include <sl/systems/PhysicsSystem.hpp>
#include <sl/systems/AnimationSystem.hpp>
#include <allegro5/allegro_native_dialog.h>

#include "GameState.hpp"

class Sandbox : public sl::Application
{
public:
	Sandbox(const std::string& config, std::function<void(std::ofstream&)> newConfig) : sl::Application(config, newConfig)
	{
		m_world->registerSystem<sl::RenderSystem>(10, 50);
		m_world->registerSystem<sl::CameraSystem>();
		m_world->registerSystem<sl::PhysicsSystem>("", m_configReader->lookup<float>(config, "box2d", "ups"), m_configReader->lookup<int>(config, "box2d", "velocityIterations"), m_configReader->lookup<int>(config, "box2d", "positionIterations"));
		m_world->registerSystem<sl::AnimationSystem>();

		m_stateMachine->addState<GameState>("game");
		m_stateMachine->push("game");

		entt::DefaultRegistry::entity_type cameraEntity = m_world->m_registry.create();
		m_world->m_registry.assign<sl::CameraTag>(entt::tag_t{}, cameraEntity, sl::Rect<float, int>{ 0, 0, 896, 576 });
	}
};

int main(int argc, char **argv)
{
	bool restart = false;

	do
	{
		restart = false;

		{
			try
			{
				Sandbox sandbox("bin/config.cfg", [](std::ofstream& newConfig)
				{
					newConfig << "[graphics]\n";
					newConfig << "width = 640\n";
					newConfig << "height = 480\n";
					newConfig << "fullscreen = false\n";
					newConfig << "msaa = true\n";
					newConfig << "msaaValue = 2\n";
					newConfig << "title = Sandbox\n";
					newConfig << "icon = icon.png\n";
					newConfig << "atlasPowerOf = 13\n";
					newConfig << "shaderScript = scripts/shaders.lua\n";
					newConfig << std::endl;

					newConfig << "[box2d]\n";
					newConfig << "gravity = 9.81\n";
					newConfig << "ups = 60.0\n";
					newConfig << "velocityIterations = 8\n";
					newConfig << "positionIterations = 3\n";
					newConfig << std::endl;

					newConfig << "[font]\n";
					newConfig << "fontScript = scripts/fonts.lua\n";
					newConfig << std::endl;

					newConfig << "[audio]\n";
					newConfig << "musicScript = scripts/music.lua\n";
					newConfig << "soundScript = scripts/sound.lua\n";
					newConfig << "reserveSamples = 32\n";
					newConfig << std::endl;

					newConfig << "# see allegro key codes\n";
					newConfig << "[keys]\n";
					newConfig << "forward = 23\n";
					newConfig << "backward = 19\n";
					newConfig << "left = 1\n";
					newConfig << "right = 4\n";
					newConfig << "quit = 59\n";
					newConfig << std::endl;

					newConfig << "[debug]\n";
					newConfig << "isDisabled = false\n";
					newConfig << std::endl;

					newConfig << "[fs]\n";
					newConfig << "writeDir = bin/assets/\n";
					newConfig << std::endl;

					newConfig << "[archives]\n";
					newConfig << "bin/assets/\n";
					newConfig << "bin/data.zip\n";
					newConfig << std::endl;
				});

				restart = sandbox.run();
			}
			catch (const std::exception& e)
			{
				LOG_S(INFO) << "EXCEPTION OUTPUT: " << e.what();
				al_show_native_message_box(nullptr, "Runtime Exception", "Error Message:", e.what(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			}
			catch (...)
			{
				LOG_S(WARNING) << "Threw an unknown exception. Why are you not inheriting from std::exception?";
			}
		}

	} while (restart);
		

	return 0;
}