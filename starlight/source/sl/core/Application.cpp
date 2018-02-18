///
/// Application.cpp
/// starlight
///
/// Created by reworks on 08/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "sl/fs/VFS.hpp"
#include "sl/core/World.hpp"
#include "sl/utils/Time.hpp"
#include "sl/events/Keys.hpp"
#include "sl/math/Vector3.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/StateManager.hpp"
#include "sl/events/EventManager.hpp"
#include "sl/utils/ConfigReader.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/physics/Box2DManager.hpp"
#include "sl/resources/SoundPlayer.hpp"
#include "sl/resources/MusicPlayer.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/resources/ShaderLibrary.hpp"
#include "sl/components/TextComponent.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/SpriteComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "Application.hpp"

namespace sl
{
	Application::Application(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig)
	{
		std::srand(std::time(nullptr));

		std::string lname = "logs/log_" + time::getFormattedTime() + ".log";
		loguru::add_file(lname.c_str(), loguru::Append, loguru::Verbosity_MAX);
		loguru::set_fatal_handler([](const loguru::Message& message)
		{
			al_show_native_message_box(NULL, "FATAL", message.prefix, message.message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
			throw new std::exception(message.message);
		});

		LOG_S(INFO) << "App init.";

		al_install_system(ALLEGRO_VERSION_INT, atexit);
		al_install_keyboard();
		al_install_mouse();
		al_install_audio();

		al_init_font_addon();
		al_init_ttf_addon();
		al_init_image_addon();
		al_init_video_addon();
		al_init_acodec_addon();
		al_init_primitives_addon();
		al_init_native_dialog_addon();

		VFS::make(archive);
		ConfigReader::make(config, newConfig);

		Window::make(ConfigReader::inst()->lookup<int>(config, "graphics", "width"), ConfigReader::inst()->lookup<int>(config, "graphics", "height"), ConfigReader::inst()->lookup<bool>(config, "graphics", "fullscreen"), ConfigReader::inst()->lookup<bool>(config, "graphics", "msaa"), ConfigReader::inst()->lookup<int>(config, "graphics", "msaaValue"), ConfigReader::inst()->lookup<std::string>(config, "graphics", "title"), ConfigReader::inst()->lookup<std::string>(config, "graphics", "icon"));

		World::make();
		//DebugInterface::make(m_window->getDisplay());
		StateManager::make();
		TextureAtlas::make(ConfigReader::inst()->lookup<size_t>(config, "graphics", "atlasPowerOf"));
		FontBook::make(ConfigReader::inst()->lookup<std::string>(config, "fontmanager", "fontScript"));
		ShaderLibrary::make(ConfigReader::inst()->lookup<std::string>(config, "graphics", "shaderScript"));
		MusicPlayer::make(ConfigReader::inst()->lookup<std::string>(config, "audio", "musicScript"));
		SoundPlayer::make(ConfigReader::inst()->lookup<std::string>(config, "audio", "soundScript"));
		Box2DManager::make(ConfigReader::inst()->lookup<float32>(config, "box2d", "gravity"));
		EventManager::make();

		Keys::KEY_FORWARD = ConfigReader::inst()->lookup<int>(config, "keys", "forward");
		Keys::KEY_BACKWARD = ConfigReader::inst()->lookup<int>(config, "keys", "backward");
		Keys::KEY_LEFT = ConfigReader::inst()->lookup<int>(config, "keys", "left");
		Keys::KEY_RIGHT = ConfigReader::inst()->lookup<int>(config, "keys", "right");
		Keys::KEY_QUIT = ConfigReader::inst()->lookup<int>(config, "keys", "quit");

		#ifdef NDEBUG
			// DebugInterface::get()->disable(true);
		#endif

		al_reserve_samples(ConfigReader::inst()->lookup<int>(config, "audio", "reserveSamples"));
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		// al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA) -> apparently default allegro setting???

		Box2DManager::inst()->m_world->SetContactListener(&m_engineCallbacks);

		m_workaround.setRegistry(&(World::inst()->m_registry));

		LOG_S(INFO) << "Registering lua usertypes." << "\n";

		World::inst()->m_lua.new_usertype<std::uint32_t>("uint32_t");
		World::inst()->m_lua.new_usertype<std::uint16_t>("uint16_t");
		World::inst()->m_lua.new_usertype<entt::Entity>("entity");

		World::inst()->m_lua.new_usertype<Vector2<int>>("Vector2i",
			sol::constructors<Vector2<int>(), Vector2<int>(int, int)>(),
			"transpose", &Vector2<int>::transpose,
			"x", &Vector2<int>::m_x,
			"y", &Vector2<int>::m_y
		);

		World::inst()->m_lua.new_usertype<Vector2<float>>("Vector2f",
			sol::constructors<Vector2<float>(), Vector2<float>(float, float)>(),
			"transpose", &Vector2<float>::transpose,
			"x", &Vector2<float>::m_x,
			"y", &Vector2<float>::m_y
		);

		World::inst()->m_lua.new_usertype<Vector3<int>>("Vector3i",
			sol::constructors<Vector3<int>(), Vector3<int>(int, int, int)>(),
			"x", &Vector3<int>::m_x,
			"y", &Vector3<int>::m_y,
			"z", &Vector3<int>::m_z
			);

		World::inst()->m_lua.new_usertype<Vector3<float>>("Vector3f",
			sol::constructors<Vector3<float>(), Vector3<float>(float, float, float)>(),
			"x", &Vector3<float>::m_x,
			"y", &Vector3<float>::m_y,
			"z", &Vector3<float>::m_z
		);

		World::inst()->m_lua.new_usertype<Rect<int>>("iRect",
			sol::constructors<Rect<int>(), Rect<int>(int, int, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(int, int)>(&Rect<int>::contains), sol::resolve<bool(const Rect<int>&)>(&Rect<int>::contains)),
			"x", &Rect<int>::m_x,
			"y", &Rect<int>::m_y,
			"width", &Rect<int>::m_width,
			"height", &Rect<int>::m_height
		);

		World::inst()->m_lua.new_usertype<Rect<float>>("fRect",
			sol::constructors<Rect<float>(), Rect<float>(float, float, float, float)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float>::contains), sol::resolve<bool(const Rect<float>&)>(&Rect<float>::contains)),
			"x", &Rect<float>::m_x,
			"y", &Rect<float>::m_y,
			"width", &Rect<float>::m_width,
			"height", &Rect<float>::m_height
		);

		World::inst()->m_lua.new_usertype<Rect<float, int>>("fiRect",
			sol::constructors<Rect<float, int>(), Rect<float, int>(float, float, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float, int>::contains), sol::resolve<bool(const Rect<float, int>&)>(&Rect<float, int>::contains)),
			"x", &Rect<float, int>::m_x,
			"y", &Rect<float, int>::m_y,
			"width", &Rect<float, int>::m_width,
			"height", &Rect<float, int>::m_height
		);

		World::inst()->m_lua.new_usertype<AnimationComponent>("AnimationComponent",
			sol::constructors<AnimationComponent(entt::Entity, const sol::table&)>(),
			"changeAnimation", &AnimationComponent::changeAnimation,
			"play", sol::overload(sol::resolve<void(void)>(&AnimationComponent::play), sol::resolve<void(std::string_view)>(&AnimationComponent::play)),
			"pause", &AnimationComponent::pause,
			"stop", &AnimationComponent::stop,
			"m_isPaused", &AnimationComponent::m_isPaused,
			"m_currentFrameTime", &AnimationComponent::m_currentFrameTime,
			"m_activeAnimation", &AnimationComponent::m_activeAnimation,
			"m_animations", &AnimationComponent::m_animations
		);

		World::inst()->m_lua.new_usertype<ParallaxComponent>("ParallaxComponent",
			sol::constructors<ParallaxComponent(entt::Entity, const sol::table&)>(),
			"verticalSpeed", &ParallaxComponent::m_verticalSpeed,
			"horizontalSpeed", &ParallaxComponent::m_horizontalSpeed
		);

		World::inst()->m_lua.new_usertype<ParticleComponent>("ParticleComponent",
			sol::constructors<ParticleComponent(float, float, float, float, const std::string&)>(),
			"fade", &ParticleComponent::m_fade,
			"alpha", &ParticleComponent::m_alpha,
			"direction", &ParticleComponent::m_direction,
			"id", &ParticleComponent::m_id
		);

		World::inst()->m_lua.new_usertype<SpriteComponent>("SpriteComponent",
			sol::constructors<SpriteComponent(entt::Entity, const sol::table&), SpriteComponent(const std::string&, float)>(),
			"opacity", &SpriteComponent::m_opacity,
			"spriteName", &SpriteComponent::m_spriteName
		);

		World::inst()->m_lua.new_usertype<RenderComponent>("RenderComponent",
			sol::constructors<RenderComponent(entt::Entity, const sol::table&)>(),
			"renderTypes", &RenderComponent::m_renderTypes
		);

		World::inst()->m_lua.new_usertype<TransformComponent>("TransformComponent",
			sol::constructors<TransformComponent(entt::Entity, const sol::table&), TransformComponent(int, float, const Rect<float, int>&)>(),
			"layer", &TransformComponent::m_layer,
			"angle", &TransformComponent::m_angle,
			"rect", &TransformComponent::m_rect
		);

		World::inst()->m_lua.new_usertype<TextComponent>("TextComponent",
			sol::constructors<TextComponent(entt::Entity, const sol::table&)>(),
			"id", &TextComponent::m_id
		);

		World::inst()->m_lua.new_usertype<PhysicsComponent>("PhysicsComponent",
			sol::constructors<PhysicsComponent(entt::Entity, const sol::table&)>()
		);

		World::inst()->m_lua.new_usertype<Sol2enttWorkaround>("Registry",
			sol::constructors<Sol2enttWorkaround()>(),
			/*"create", &Sol2enttWorkaround::create,*/
			"destroy", &Sol2enttWorkaround::destroy,
			"getAnimationComponent", &Sol2enttWorkaround::get<AnimationComponent>,
			"getParallaxComponent", &Sol2enttWorkaround::get<ParallaxComponent>,
			"getParticleComponent", &Sol2enttWorkaround::get<ParticleComponent>,
			"getPhysicsComponent", &Sol2enttWorkaround::get<PhysicsComponent>,
			"getRenderComponent", &Sol2enttWorkaround::get<RenderComponent>,
			"getSpriteComponent", &Sol2enttWorkaround::get<SpriteComponent>,
			"getTextComponent", &Sol2enttWorkaround::get<TextComponent>,
			"getTransformComponent", &Sol2enttWorkaround::get<TransformComponent>
		);

		World::inst()->m_lua["registry"] = m_workaround;
	}

	Application::~Application()
	{
		EventManager::destroy();
		Box2DManager::destroy();
		SoundPlayer::destroy();
		MusicPlayer::destroy();
		ShaderLibrary::destroy();
		FontBook::destroy();
		TextureAtlas::destroy();
		StateManager::destroy();
		//DebugInterface::destroy();
		World::destroy();
		Window::destroy();
		ConfigReader::destroy();
		VFS::destroy();

		al_shutdown_native_dialog_addon();
		al_shutdown_primitives_addon();
		al_shutdown_video_addon();
		al_shutdown_image_addon();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();

		al_uninstall_audio();
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_uninstall_system();
	}

	int Application::run()
	{
		#ifndef NDEBUG
			int frames = 0;
			int updates = 0;
		#endif

		double timePerFrame = 1.0 / 60.0;
		std::uint64_t timer = 0;

		World* world = World::inst();
		Window* window = Window::inst();
		StateManager* stateManager = StateManager::inst();
		//DebugInterface* debugInterface = DebugInterface::inst();
		EventManager* eventManager = EventManager::inst();

		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		
		al_register_event_source(eventManager->m_queue, al_get_display_event_source(window->getDisplay()));
		al_register_event_source(eventManager->m_queue, al_get_mouse_event_source());
		al_register_event_source(eventManager->m_queue, al_get_keyboard_event_source());
		al_register_event_source(eventManager->m_queue, al_get_timer_event_source(clock));

		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		al_start_timer(clock);

		stateManager->load();

		while (window->isOpen())
		{
			ALLEGRO_EVENT ev;
			while (al_get_next_event(eventManager->m_queue, &ev))
			{
				world->event(&ev);
				stateManager->event(&ev);
				//debugInterface->event(&ev);
				
				switch (ev.type)
				{
				case ALLEGRO_EVENT_TIMER:
					stateManager->update(timePerFrame);
					world->update(timePerFrame);

					#ifndef NDEBUG
						updates++;	
					#endif

					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					window->close();
					break;

				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					//ImGui_ImplA5_InvalidateDeviceObjects();
					//al_acknowledge_resize(window->getDisplay());
					//Imgui_ImplA5_CreateDeviceObjects();
					break;
				}
			}

			//debugInterface->newFrame();
			//debugInterface->displayMenu();

			window->clear(255, 255, 255);

			stateManager->render();
			//debugInterface->render();

			window->display();

			#ifndef NDEBUG
				frames++;

				if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
				{
					timer += 1000;
					LOG_S(INFO) << updates << " ups, " << frames << " fps";

					updates = 0;
					frames = 0;
				}
			#endif
		}

		stateManager->unload();

		al_stop_timer(clock);
		al_destroy_timer(clock);

		LOG_S(INFO) << "App close.";

		return EXIT_SUCCESS;
	}
}