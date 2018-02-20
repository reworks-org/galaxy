///
/// main.cpp
/// sandbox
///
/// Created by reworks on 08/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <sl/tags/CameraTag.hpp>
#include <sl/core/Application.hpp>
#include <sl/mapping/TMXUtils.hpp>

#include "GameState.hpp"

class Sandbox : public sl::Application
{
public:
	Sandbox(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig) : sl::Application(archive, config, newConfig)
	{
		sl::tmxutils::LINE_THICKNESS = 2.5f;

		m_stateManager->setState(GameState::inst());
		m_debugInterface->setReloadState(GameState::inst(), []()
		{
			LOG_S(INFO) << "Reloaded state.";
		});
	}
};

int main(int argc, char **argv)
{
	Sandbox sandbox("bin/data.zip", "config.cfg", [](std::ofstream& newConfig)
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

	return sandbox.run();
}