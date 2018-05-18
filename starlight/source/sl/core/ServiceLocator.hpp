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
	class StateMachine;
	class TextureAtlas;
	class VirtualFS;
	#ifndef NDEBUG
		class DebugInterface;
	#endif

	///
	/// Provides access to services in the framework.
	/// This does not follow typical member variable naming (no 'm_' prefix)
	/// So that the code looks nicer when using the Locator. E.g:
	/// Locator::world-> vs Locator::world->
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
		static inline World* world = nullptr;
		static inline Window* window = nullptr;
		static inline Box2DManager* box2dManager = nullptr;
		static inline ConfigReader* configReader = nullptr;
		static inline EventManager* eventManager = nullptr;
		static inline FontBook* fontBook = nullptr;
		static inline MusicPlayer* musicPlayer = nullptr;
		static inline ShaderLibrary* shaderLibrary = nullptr;
		static inline SoundPlayer* soundPlayer = nullptr;
		static inline StateMachine* stateMachine = nullptr;
		static inline TextureAtlas* textureAtlas = nullptr;
		static inline VirtualFS* virtualFS = nullptr;
		#ifndef NDEBUG
			static inline DebugInterface* debugInterface = nullptr;
		#endif
	};
}

#endif