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

#include "sl/utils/Time.hpp"
#include "sl/events/Keys.hpp"
#include "sl/math/Vector3.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/imgui/imgui_impl_a5.h"
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
			throw std::runtime_error(message.message);
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

		m_configReader = std::make_unique<ConfigReader>(config, newConfig);
		Locator::m_configReader = m_configReader.get();

		m_virtualFS = std::make_unique<VirtualFS>(archive);
		Locator::m_virtualFS = m_virtualFS.get();

		m_window = std::make_unique<Window>(m_configReader->lookup<int>(config, "graphics", "width"), m_configReader->lookup<int>(config, "graphics", "height"), m_configReader->lookup<bool>(config, "graphics", "fullscreen"), m_configReader->lookup<bool>(config, "graphics", "msaa"), m_configReader->lookup<int>(config, "graphics", "msaaValue"), m_configReader->lookup<std::string>(config, "graphics", "title"), m_configReader->lookup<std::string>(config, "graphics", "icon"));
		Locator::m_window = m_window.get();

		al_reserve_samples(m_configReader->lookup<int>(config, "audio", "reserveSamples"));
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		// Apparently this is the default allegro settings. This is here for a reference.
		// al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA)

		m_world = std::make_unique<World>();
		Locator::m_world = m_world.get();

		m_debugInterface = std::make_unique<DebugInterface>(m_window->getDisplay(), m_configReader->lookup<bool>(config, "debug", "isDisabled"));
		Locator::m_debugInterface = m_debugInterface.get();

		m_stateManager = std::make_unique<StateManager>();
		Locator::m_stateManager = m_stateManager.get();

		m_textureAtlas = std::make_unique<TextureAtlas>(m_configReader->lookup<size_t>(config, "graphics", "atlasPowerOf"));
		Locator::m_textureAtlas = m_textureAtlas.get();

		m_fontBook = std::make_unique<FontBook>(m_configReader->lookup<std::string>(config, "font", "fontScript"));
		Locator::m_fontBook = m_fontBook.get();

		m_shaderLibrary = std::make_unique<ShaderLibrary>(m_configReader->lookup<std::string>(config, "graphics", "shaderScript"));
		Locator::m_shaderLibrary = m_shaderLibrary.get();

		m_musicPlayer = std::make_unique<MusicPlayer>(m_configReader->lookup<std::string>(config, "audio", "musicScript"));
		Locator::m_musicPlayer = m_musicPlayer.get();

		m_soundPlayer = std::make_unique<SoundPlayer>(m_configReader->lookup<std::string>(config, "audio", "soundScript"));
		Locator::m_soundPlayer = m_soundPlayer.get();

		m_box2dManager = std::make_unique<Box2DManager>(m_configReader->lookup<float32>(config, "box2d", "gravity"));
		Locator::m_box2dManager = m_box2dManager.get();

		m_eventManager = std::make_unique<EventManager>();
		Locator::m_eventManager = m_eventManager.get();

		Keys::KEY_FORWARD = m_configReader->lookup<int>(config, "keys", "forward");
		Keys::KEY_BACKWARD = m_configReader->lookup<int>(config, "keys", "backward");
		Keys::KEY_LEFT = m_configReader->lookup<int>(config, "keys", "left");
		Keys::KEY_RIGHT = m_configReader->lookup<int>(config, "keys", "right");
		Keys::KEY_QUIT = m_configReader->lookup<int>(config, "keys", "quit");

		m_box2dManager->m_b2world->SetContactListener(&m_engineCallbacks);
		m_workaround.setRegistry(&(m_world->m_registry));

		LOG_S(INFO) << "Registering lua usertypes." << "\n";

		m_world->m_lua.new_usertype<std::uint32_t>("uint32_t");
		m_world->m_lua.new_usertype<std::uint16_t>("uint16_t");
		m_world->m_lua.new_usertype<entt::Entity>("entity");

		m_world->m_lua.new_usertype<Vector2<int>>("Vector2i",
			sol::constructors<Vector2<int>(), Vector2<int>(int, int)>(),
			"transpose", &Vector2<int>::transpose,
			"x", &Vector2<int>::m_x,
			"y", &Vector2<int>::m_y
		);

		m_world->m_lua.new_usertype<Vector2<float>>("Vector2f",
			sol::constructors<Vector2<float>(), Vector2<float>(float, float)>(),
			"transpose", &Vector2<float>::transpose,
			"x", &Vector2<float>::m_x,
			"y", &Vector2<float>::m_y
		);

		m_world->m_lua.new_usertype<Vector3<int>>("Vector3i",
			sol::constructors<Vector3<int>(), Vector3<int>(int, int, int)>(),
			"x", &Vector3<int>::m_x,
			"y", &Vector3<int>::m_y,
			"z", &Vector3<int>::m_z
		);

		m_world->m_lua.new_usertype<Vector3<float>>("Vector3f",
			sol::constructors<Vector3<float>(), Vector3<float>(float, float, float)>(),
			"x", &Vector3<float>::m_x,
			"y", &Vector3<float>::m_y,
			"z", &Vector3<float>::m_z
		);

		m_world->m_lua.new_usertype<Rect<int>>("iRect",
			sol::constructors<Rect<int>(), Rect<int>(int, int, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(int, int)>(&Rect<int>::contains), sol::resolve<bool(const Rect<int>&)>(&Rect<int>::contains)),
			"x", &Rect<int>::m_x,
			"y", &Rect<int>::m_y,
			"width", &Rect<int>::m_width,
			"height", &Rect<int>::m_height
		);

		m_world->m_lua.new_usertype<Rect<float>>("fRect",
			sol::constructors<Rect<float>(), Rect<float>(float, float, float, float)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float>::contains), sol::resolve<bool(const Rect<float>&)>(&Rect<float>::contains)),
			"x", &Rect<float>::m_x,
			"y", &Rect<float>::m_y,
			"width", &Rect<float>::m_width,
			"height", &Rect<float>::m_height
		);

		m_world->m_lua.new_usertype<Rect<float, int>>("fiRect",
			sol::constructors<Rect<float, int>(), Rect<float, int>(float, float, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float, int>::contains), sol::resolve<bool(const Rect<float, int>&)>(&Rect<float, int>::contains)),
			"x", &Rect<float, int>::m_x,
			"y", &Rect<float, int>::m_y,
			"width", &Rect<float, int>::m_width,
			"height", &Rect<float, int>::m_height
		);

		m_world->m_lua.new_usertype<AnimationComponent>("AnimationComponent",
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

		m_world->m_lua.new_usertype<ParallaxComponent>("ParallaxComponent",
			sol::constructors<ParallaxComponent(entt::Entity, const sol::table&)>(),
			"verticalSpeed", &ParallaxComponent::m_verticalSpeed,
			"horizontalSpeed", &ParallaxComponent::m_horizontalSpeed
		);

		m_world->m_lua.new_usertype<ParticleComponent>("ParticleComponent",
			sol::constructors<ParticleComponent(float, float, float, float, const std::string&)>(),
			"fade", &ParticleComponent::m_fade,
			"alpha", &ParticleComponent::m_alpha,
			"direction", &ParticleComponent::m_direction,
			"id", &ParticleComponent::m_id
		);

		m_world->m_lua.new_usertype<SpriteComponent>("SpriteComponent",
			sol::constructors<SpriteComponent(entt::Entity, const sol::table&), SpriteComponent(const std::string&, float)>(),
			"opacity", &SpriteComponent::m_opacity,
			"spriteName", &SpriteComponent::m_spriteName
		);

		m_world->m_lua.new_usertype<RenderComponent>("RenderComponent",
			sol::constructors<RenderComponent(entt::Entity, const sol::table&)>(),
			"renderTypes", &RenderComponent::m_renderTypes
		);

		m_world->m_lua.new_usertype<TransformComponent>("TransformComponent",
			sol::constructors<TransformComponent(entt::Entity, const sol::table&), TransformComponent(int, float, const Rect<float, int>&)>(),
			"layer", &TransformComponent::m_layer,
			"angle", &TransformComponent::m_angle,
			"rect", &TransformComponent::m_rect
		);

		m_world->m_lua.new_usertype<TextComponent>("TextComponent",
			sol::constructors<TextComponent(entt::Entity, const sol::table&)>(),
			"id", &TextComponent::m_id
		);

		m_world->m_lua.new_usertype<PhysicsComponent>("PhysicsComponent",
			sol::constructors<PhysicsComponent(entt::Entity, const sol::table&)>()
		);

		m_world->m_lua.new_usertype<Sol2enttWorkaround>("Registry",
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

		m_world->m_lua["registry"] = m_workaround;
	}

	Application::~Application()
	{
		m_eventManager.reset();
		m_box2dManager.reset();
		m_soundPlayer.reset();
		m_musicPlayer.reset();
		m_shaderLibrary.reset();
		m_fontBook.reset();
		m_textureAtlas.reset();
		m_stateManager.reset();
		m_debugInterface.reset();
		m_world.reset();
		m_window.reset();
		m_configReader.reset();
		m_virtualFS.reset();

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
			std::uint64_t timer = 0;
		#endif

		double timePerFrame = 1.0 / 60.0;

		al_register_event_source(m_eventManager->m_queue, al_get_display_event_source(m_window->getDisplay()));
		al_register_event_source(m_eventManager->m_queue, al_get_mouse_event_source());
		al_register_event_source(m_eventManager->m_queue, al_get_keyboard_event_source());
		
		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		al_register_event_source(m_eventManager->m_queue, al_get_timer_event_source(clock));
		al_start_timer(clock);
		
		m_stateManager->load();

		#ifndef NDEBUG
			timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		#endif

		while (m_window->isOpen())
		{
			ALLEGRO_EVENT ev;
			while (al_get_next_event(m_eventManager->m_queue, &ev))
			{
				m_world->event(&ev);
				m_stateManager->event(&ev);
				m_debugInterface->event(&ev);
				
				switch (ev.type)
				{
				case ALLEGRO_EVENT_TIMER:
					m_stateManager->update(timePerFrame);
					m_world->update(timePerFrame);

					#ifndef NDEBUG
						updates++;	
					#endif

					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					m_window->close();
					break;

				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					ImGui_ImplA5_InvalidateDeviceObjects();
					al_acknowledge_resize(m_window->getDisplay());
					Imgui_ImplA5_CreateDeviceObjects();
					break;
				}
			}

			m_debugInterface->newFrame();
			m_debugInterface->displayMenu();

			m_window->clear(255, 255, 255);

			m_stateManager->render();
			m_debugInterface->render();

			m_window->display();

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

		m_stateManager->unload();

		al_stop_timer(clock);
		al_destroy_timer(clock);

		LOG_S(INFO) << "App close.";

		return EXIT_SUCCESS;
	}
}