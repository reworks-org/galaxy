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
#include <sl/systems/PhysicsSystem.hpp>
#include <sl/systems/AnimationSystem.hpp>
#include <sl/systems/ScrollingBackgroundSystem.hpp>
#include <allegro5/allegro_native_dialog.h>

#include "GameState.hpp"

class Sandbox : public galaxyApplication
{
public:
	Sandbox(const std::string& config, std::function<void(std::ofstream&)> newConfig) : galaxyApplication(config, newConfig)
	{
		m_world->registerSystem<galaxyRenderSystem>(20, 100);
		m_world->registerSystem<galaxyPhysicsSystem>("", m_configReader->lookup<float>(config, "box2d", "ups"), m_configReader->lookup<int>(config, "box2d", "velocityIterations"), m_configReader->lookup<int>(config, "box2d", "positionIterations"));
		m_world->registerSystem<galaxyAnimationSystem>();
		m_world->registerSystem<galaxyScrollingBackgroundSystem>();

		m_stateMachine->createState<GameState>("game");
		m_stateMachine->push("game");

		entt::DefaultRegistry::entity_type cameraEntity = m_world->m_registry.create();
		m_world->m_registry.assign<galaxyCameraTag>(entt::tag_t{}, cameraEntity, galaxyRect<float, int>{ 0, 0, 1280, 720 }); //896, 576
	}
};

int main(int argc, char **argv)
{
	bool restart = false;

	do
	{
		restart = false;

		{
			//try
			//{
				Sandbox sandbox("bin/config.cfg", [](std::ofstream& newConfig)
				{
					newConfig << "[graphics]\n";
					newConfig << "width = 1280\n";
					newConfig << "height = 720\n";
					newConfig << "fullscreen = false\n";
					newConfig << "msaa = true\n";
					newConfig << "msaaValue = 2\n";
					newConfig << "grabMouse = true\n";
					newConfig << "title = Sandbox\n";
					newConfig << "icon = icon.png\n";
					newConfig << "atlasPowerOf = 13\n";
					newConfig << "shaderScript = shaders.lua\n";
					newConfig << "nullTexture = null\n";
					newConfig << std::endl;

					newConfig << "[box2d]\n";
					newConfig << "gravity = 9.81\n";
					newConfig << "ups = 60.0\n";
					newConfig << "velocityIterations = 8\n";
					newConfig << "positionIterations = 3\n";
					newConfig << std::endl;

					newConfig << "[font]\n";
					newConfig << "fontScript = fonts.lua\n";
					newConfig << std::endl;

					newConfig << "[audio]\n";
					newConfig << "musicScript = music.lua\n";
					newConfig << "soundScript = sound.lua\n";
					newConfig << "reserveSamples = 32\n";
					newConfig << "nullMusic = null\n";
					newConfig << "nullSound = null\n";
					newConfig << std::endl;

					newConfig << "[fs]\n";
					newConfig << "writeDir = bin/assets/\n";
					newConfig << "scriptFolderPath = scripts/\n";
					newConfig << "textureFolderPath = textures/\n";
					newConfig << "musicFolderPath = music/\n";
					newConfig << "soundFolderPath = sound/\n";
					newConfig << "fontFolderPath = fonts/\n";
					newConfig << std::endl;

					newConfig << "[archives]\n";
					newConfig << "bin/assets/";
					newConfig << std::endl;
				});

				restart = sandbox.run();
			//}
			//catch (const std::exception& e)
			//{
				//LOG_S(INFO) << "EXCEPTION OUTPUT: " << e.what();
				//al_show_native_message_box(nullptr, "Runtime Exception", "Error Message:", e.what(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
			//}
			//catch (...)
			//{
				//LOG_S(WARNING) << "Threw an unknown exception. Why are you not inheriting from std::exception?";
			//}
		}

	} while (restart);
		
	return 0;
}