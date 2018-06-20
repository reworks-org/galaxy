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
	// This avoids mass includes.
	class World;
	class Window;
	class Box2DHelper;
	class ConfigReader;
	class EventManager;
	class FontBook;
	class MusicPlayer;
	class ShaderLibrary;
	class SoundPlayer;
	class StateMachine;
	class TextureAtlas;
	class VirtualFS;
	class DebugInterface;

	///
	/// Provides access to services in the framework.
	/// This does not follow typical member variable naming (no 'm_' prefix)
	/// So that the code looks nicer when using the Locator. E.g:
	/// Locator::m_world-> vs Locator::world->
	///
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
		///
		/// Pointer to World service.
		///
		static inline World* world = nullptr;
		
		///
		/// Pointer to Window service.
		///
		static inline Window* window = nullptr;
		
		///
		/// Pointer to Box2DManager service.
		///
		static inline Box2DHelper* box2dHelper = nullptr;
		
		///
		/// Pointer to ConfigReader service.
		///
		static inline ConfigReader* configReader = nullptr;

		///
		/// Pointer to EventManager service.
		///
		static inline EventManager* eventManager = nullptr;
		
		///
		/// Pointer to FontBook service.
		///
		static inline FontBook* fontBook = nullptr;

		///
		/// Pointer to MusicPlayer service.
		///		
		static inline MusicPlayer* musicPlayer = nullptr;
		
		///
		/// Pointer to ShaderLibrary service.
		///
		static inline ShaderLibrary* shaderLibrary = nullptr;
		
		///
		/// Pointer to SoundPlayer service.
		///
		static inline SoundPlayer* soundPlayer = nullptr;
		
		///
		/// Pointer to StateMachine service.
		///
		static inline StateMachine* stateMachine = nullptr;
		
		///
		/// Pointer to TextureAtlas service.
		///
		static inline TextureAtlas* textureAtlas = nullptr;
		
		///
		/// Pointer to VirtualFS service.
		///
		static inline VirtualFS* virtualFS = nullptr;
		
		///
		/// Pointer to DebugInterface service.
		///
		static inline DebugInterface* debugInterface = nullptr;
	};
}

#endif