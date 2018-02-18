///
/// ServiceLocator.hpp
/// starlight
///
/// Created by reworks on 18/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SERVICELOCATOR_HPP_
#define STARLIGHT_SERVICELOCATOR_HPP_

namespace sl
{
	class World;
	class Window;
	class Box2DManager;
	class ConfigReader;
	class EventManager;
	class FontBook;
	class MusicPlayer;
	class ShaderLibrary;
	class SoundPlayer;
	class StateManager;
	class TextureAtlas;
	class VirtualFS;
	//class DebugInterface;

	class Locator
	{
	public:
		///
		/// Destructor.
		///
		virtual ~Locator() = default;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Locator() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Locator(const Locator&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Locator(Locator&&) = delete;

	public:
		static World* m_world;
		static Window* m_window;
		static Box2DManager* m_box2dManager;
		static ConfigReader* m_configReader;
		static EventManager* m_eventManager;
		static FontBook* m_fontBook;
		static MusicPlayer* m_musicPlayer;
		static ShaderLibrary* m_shaderLibrary;
		static SoundPlayer* m_soundPlayer;
		static StateManager* m_stateManager;
		static TextureAtlas* m_textureAtlas;
		static VirtualFS* m_virtualFS;
		//static DebugInterface* m_debugInterface;
	};
}

#endif