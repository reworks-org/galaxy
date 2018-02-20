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
	class DebugInterface;

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
		static inline World* m_world = nullptr;
		static inline Window* m_window = nullptr;
		static inline Box2DManager* m_box2dManager = nullptr;
		static inline ConfigReader* m_configReader = nullptr;
		static inline EventManager* m_eventManager = nullptr;
		static inline FontBook* m_fontBook = nullptr;
		static inline MusicPlayer* m_musicPlayer = nullptr;
		static inline ShaderLibrary* m_shaderLibrary = nullptr;
		static inline SoundPlayer* m_soundPlayer = nullptr;
		static inline StateManager* m_stateManager = nullptr;
		static inline TextureAtlas* m_textureAtlas = nullptr;
		static inline VirtualFS* m_virtualFS = nullptr;
		static inline DebugInterface* m_debugInterface = nullptr;
	};
}

#endif