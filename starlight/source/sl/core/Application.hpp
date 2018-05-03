///
/// Application.hpp
/// starlight
///
/// Created by reworks on 08/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_APPLICATION_HPP_
#define STARLIGHT_APPLICATION_HPP_

#include <fstream>

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/StateManager.hpp"
#include "sl/utils/ConfigReader.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/events/EventManager.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/resources/SoundPlayer.hpp"
#include "sl/resources/MusicPlayer.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/physics/Box2DCallbacks.hpp"
#include "sl/debug/DebugInterface.hpp"
#include "sl/resources/ShaderLibrary.hpp"
#include "sl/scripting/Sol2enttWorkaround.hpp"

namespace sl
{
	class Application
	{
	public:
		///
		/// Cleans up engine related memory usage.
		/// 
		virtual ~Application();

		///
		/// Return app->run() from your main method.
		///
		/// \return Returns EXIT_SUCCESS.
		/// 
		virtual int run() final;

	protected:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// Also calls std::srand(std::time(nullptr)) for you.
		/// 
		/// \param archives Array of archives to load. This can be directorys or archives.
		/// \param config Path to the config file.
		/// \param newConfig A function pointer to a function that contains ofstream code to write a default config file.
		///
		Application(const std::vector<std::string>& archives, const std::string& config, std::function<void(std::ofstream&)> newConfig);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Application() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Application(const Application&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Application(Application&&) = delete;

	protected:
		std::unique_ptr<World> m_world;
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Box2DManager> m_box2dManager;
		std::unique_ptr<ConfigReader> m_configReader;
		std::unique_ptr<EventManager> m_eventManager;
		std::unique_ptr<FontBook> m_fontBook;
		std::unique_ptr<MusicPlayer> m_musicPlayer;
		std::unique_ptr<ShaderLibrary> m_shaderLibrary;
		std::unique_ptr<SoundPlayer> m_soundPlayer;
		std::unique_ptr<StateManager> m_stateManager;
		std::unique_ptr<TextureAtlas> m_textureAtlas;
		std::unique_ptr<VirtualFS> m_virtualFS;
		#ifndef NDEBUG
			std::unique_ptr<DebugInterface> m_debugInterface;
		#endif

		Sol2enttWorkaround m_workaround;
		CollisionContact m_engineCallbacks;
	};
}

#endif