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
#include "GameLevel.hpp"

class Sandbox : public sl::Application
{
public:
	Sandbox(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig) : sl::Application(archive, config, newConfig)
	{
		LOG_S(INFO) << "Registering systems...";
		
		m_world->registerSystem<sl::RenderSystem>(5, 10);
		m_world->registerSystem<sl::CameraSystem>();
		m_world->registerSystem<sl::PhysicsSystem>("", m_configReader->lookup<float>(config, "box2d", "ups"), m_configReader->lookup<int>(config, "box2d", "velocityIterations"), m_configReader->lookup<int>(config, "box2d", "positionIterations"));
		m_world->registerSystem<sl::AnimationSystem>();

		LOG_S(INFO) << "Constructing sandbox...";
		entt::Entity cameraEntity = m_world->m_registry.create();
		m_world->m_registry.attach<sl::CameraTag>(cameraEntity, sl::Rect<float, int>{0.0f, 0.0f, 256, 256});

		m_stateManager->setState(GameState::inst());
		#ifndef NDEBUG
			m_debugInterface->setReloadState(GameState::inst(), []()
			{
				LOG_S(INFO) << "Reloaded state.";
			});
		#endif
	}
};

int main(int argc, char **argv)
{
	auto success = EXIT_FAILURE;
	
	try
	{
		Sandbox sandbox("bin/data.zip", "bin/config.cfg", [](std::ofstream& newConfig)
		{
			newConfig << "[graphics]" << std::endl;
			newConfig << "width = 640" << std::endl;
			newConfig << "height = 480" << std::endl;
			newConfig << "fullscreen = false" << std::endl;
			newConfig << "msaa = true" << std::endl;
			newConfig << "msaaValue = 2" << std::endl;
			newConfig << "title = Sandbox" << std::endl;
			newConfig << "icon = icon.png" << std::endl;
			newConfig << "atlasPowerOf = 13" << std::endl;
			newConfig << "shaderScript = shaders.lua" << std::endl;
			newConfig << std::endl;

			newConfig << "[box2d]" << std::endl;
			newConfig << "gravity = 9.81" << std::endl;
			newConfig << "ups = 60.0" << std::endl;
			newConfig << "velocityIterations = 8" << std::endl;
			newConfig << "positionIterations = 3" << std::endl;
			newConfig << std::endl;

			newConfig << "[font]" << std::endl;
			newConfig << "fontScript = fonts.lua" << std::endl;
			newConfig << std::endl;

			newConfig << "[audio]" << std::endl;
			newConfig << "musicScript = music.lua" << std::endl;
			newConfig << "soundScript = sound.lua" << std::endl;
			newConfig << "reserveSamples = 32" << std::endl;
			newConfig << std::endl;

			newConfig << "# see allegro key codes" << std::endl;
			newConfig << "[keys]" << std::endl;
			newConfig << "forward = 23" << std::endl;
			newConfig << "backward = 19" << std::endl;
			newConfig << "left = 1" << std::endl;
			newConfig << "right = 4" << std::endl;
			newConfig << "quit = 59" << std::endl;
			newConfig << std::endl;

			newConfig << "[debug]" << std::endl;
			newConfig << "isDisabled = false" << std::endl;
			newConfig << std::endl;
		});

		success = sandbox.run();
	}
	catch(const std::exception& e)
	{
		LOG_S(INFO) << "EXCEPTION OUTPUT: " << e.what();
		al_show_native_message_box(nullptr, "Runtime Exception", "Error Message:", e.what(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
	}
	catch (...)
	{
		LOG_S(WARNING) << "Threw an unknown exception. Why are you not inheriting from std::exception?";
	}

	return success;
}