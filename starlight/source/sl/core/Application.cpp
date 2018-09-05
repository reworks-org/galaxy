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
#include "sl/math/Vector3.hpp"
#include "sl/libs/imgui/imgui_impl_a5.h"
#include "sl/scripting/Sol2Interface.hpp"
#include "sl/components/PhysicsComponent.hpp"

#include "Application.hpp"

namespace sl
{
	Application::Application(const std::string& config, std::function<void(std::ofstream&)> newConfig)
	:m_restart(false)
	{
		// Seed pseudo-random algorithms.
		std::srand(std::time(nullptr));

		// Supposed to improve performance.
		std::ios::sync_with_stdio(false);

		// Set up logging and set loguru to throw an exception on fatal errors.
		std::string lname = "logs/" + Time::getFormattedTime() + ".log";
		loguru::add_file(lname.c_str(), loguru::Append, loguru::Verbosity_MAX);
		loguru::set_fatal_handler([](const loguru::Message& message)
		{
			throw std::runtime_error(message.message);
		});

		// Set allegro to throw loguru errors when an assert is tirggered.
		al_register_assert_handler([](const char* expr, const char* file, int line, const char* func)
		{
			LOG_S(FATAL) << "ALLEGRO ASSERT FAILURE: Expr: " << expr << " FILE: " << file << " LINE: " << line << " FUNC: " << func;
		});

		// Initialize all of allegros systems.
		// We dont use al_init() macro because we to manually have control
		// over when allegro shutsdown.
		al_install_system(ALLEGRO_VERSION_INT, nullptr);
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

		// Set up all of the difference services.
		// The services are configured based off of the config file.

		m_configReader = std::make_unique<ConfigReader>(config, newConfig);
		Locator::configReader = m_configReader.get();

		m_virtualFS = std::make_unique<VirtualFS>(m_configReader->getSection(config, "archives"));
		Locator::virtualFS = m_virtualFS.get();
		m_virtualFS->setWriteDir(m_configReader->lookup<std::string>(config, "fs", "writeDir"));

		m_window = std::make_unique<Window>(m_configReader->lookup<int>(config, "graphics", "width"), m_configReader->lookup<int>(config, "graphics", "height"), m_configReader->lookup<bool>(config, "graphics", "fullscreen"), m_configReader->lookup<bool>(config, "graphics", "msaa"), m_configReader->lookup<int>(config, "graphics", "msaaValue"), m_configReader->lookup<std::string>(config, "graphics", "title"), m_configReader->lookup<std::string>(config, "graphics", "icon"));
		Locator::window = m_window.get();

		al_reserve_samples(m_configReader->lookup<int>(config, "audio", "reserveSamples"));
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

		// Apparently this is the default allegro settings. This is here for a reference.
		// al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);

		m_world = std::make_unique<World>();
		Locator::world = m_world.get();

		m_world->m_scriptFolderPath = m_configReader->lookup<std::string>(config, "fs", "scriptFolderPath");
		m_world->m_textureFolderPath = m_configReader->lookup<std::string>(config, "fs", "textureFolderPath");
		m_world->m_musicFolderPath = m_configReader->lookup<std::string>(config, "fs", "musicFolderPath");
		m_world->m_soundFolderPath = m_configReader->lookup<std::string>(config, "fs", "soundFolderPath");

		m_debugInterface = std::make_unique<DebugInterface>(m_world->m_scriptFolderPath, m_window->getDisplay(), m_configReader->lookup<bool>(config, "debug", "isDisabled"));
		Locator::debugInterface = m_debugInterface.get();

		m_stateMachine = std::make_unique<StateMachine>();
		Locator::stateMachine = m_stateMachine.get();

		m_textureAtlas = std::make_unique<TextureAtlas>(m_world->m_textureFolderPath, m_configReader->lookup<int>(config, "graphics", "atlasPowerOf"));
		Locator::textureAtlas = m_textureAtlas.get();

		m_textureAtlas->m_nullTexture = m_configReader->lookup<std::string>(config, "graphics", "nullTexture");

		m_fontBook = std::make_unique<FontBook>(m_configReader->lookup<std::string>(config, "font", "fontScript"));
		Locator::fontBook = m_fontBook.get();

		m_shaderLibrary = std::make_unique<ShaderLibrary>(m_configReader->lookup<std::string>(config, "graphics", "shaderScript"));
		Locator::shaderLibrary = m_shaderLibrary.get();

		m_musicPlayer = std::make_unique<MusicPlayer>(m_configReader->lookup<std::string>(config, "audio", "musicScript"));
		Locator::musicPlayer = m_musicPlayer.get();

		m_musicPlayer->m_nullMusic = m_configReader->lookup<std::string>(config, "audio", "nullMusic");

		m_soundPlayer = std::make_unique<SoundPlayer>(m_configReader->lookup<std::string>(config, "audio", "soundScript"));
		Locator::soundPlayer = m_soundPlayer.get();

		m_soundPlayer->m_nullSound = m_configReader->lookup<std::string>(config, "audio", "nullSound");

		m_box2dHelper = std::make_unique<Box2DHelper>(m_configReader->lookup<float32>(config, "box2d", "gravity"));
		Locator::box2dHelper = m_box2dHelper.get();

		m_eventManager = std::make_unique<EventManager>();
		Locator::eventManager = m_eventManager.get();

		m_box2dHelper->m_b2world->SetContactListener(&m_engineCallbacks);

		// Now all the usertypes we want to access from lua are registered.
		m_world->m_lua.new_usertype<std::uint32_t>("uint32_t");
		m_world->m_lua.new_usertype<std::uint16_t>("uint16_t");
		m_world->m_lua.new_usertype<entt::DefaultRegistry::entity_type>("entity");

		// Engine usertypes.
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
			sol::constructors<AnimationComponent(const sol::table&)>(),
			"changeAnimation", &AnimationComponent::changeAnimation,
			"play", sol::overload(sol::resolve<void(void)>(&AnimationComponent::play), sol::resolve<void(const std::string&)>(&AnimationComponent::play)),
			"pause", &AnimationComponent::pause,
			"stop", &AnimationComponent::stop,
			"m_isPaused", &AnimationComponent::m_isPaused,
			"m_currentFrameTime", &AnimationComponent::m_currentFrameTime,
			"m_activeAnimation", &AnimationComponent::m_activeAnimation,
			"m_animations", &AnimationComponent::m_animations
		);

		m_world->m_lua.new_usertype<ParallaxComponent>("ParallaxComponent",
			sol::constructors<ParallaxComponent(const sol::table&)>(),
			"verticalSpeed", &ParallaxComponent::m_verticalSpeed,
			"horizontalSpeed", &ParallaxComponent::m_horizontalSpeed
		);

		m_world->m_lua.new_usertype<ParticleComponent>("ParticleComponent",
			sol::constructors<ParticleComponent(float, float, float)>(),
			"fade", &ParticleComponent::m_fade,
			"direction", &ParticleComponent::m_direction
		);

		m_world->m_lua.new_usertype<RenderComponent>("RenderComponent",
			sol::constructors<RenderComponent(const sol::table&), RenderComponent(float, const std::string&)>(),
			"opacity", &RenderComponent::m_opacity,
			"textureName", &RenderComponent::m_textureName
		);

		m_world->m_lua.new_usertype<TransformComponent>("TransformComponent",
			sol::constructors<TransformComponent(const sol::table&), TransformComponent(int, float, const Rect<float, int>&)>(),
			"layer", &TransformComponent::m_layer,
			"angle", &TransformComponent::m_angle,
			"rect", &TransformComponent::m_rect
		);

		m_world->m_lua.new_usertype<PhysicsComponent>("PhysicsComponent",
			sol::constructors<PhysicsComponent(const sol::table&)>(),
			"setFixtureEntity", &PhysicsComponent::setFixtureEntity
		);

		m_world->m_lua.new_usertype<ScrollingBackgroundComponent>("ScrollingBackgroundComponent",
			sol::constructors<ScrollingBackgroundComponent(const sol::table&), ScrollingBackgroundComponent(float)>()
		);

		m_world->m_lua.new_usertype<entt::DefaultRegistry>("Registry",
			sol::constructors<entt::DefaultRegistry()>(),
			"create", sol::resolve<entt::DefaultRegistry::entity_type(void)>(&entt::DefaultRegistry::create),
			"destroy", &Sol2Interface::enttDestroyWorkaround,
			"disable", sol::resolve<void(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::remove<EnabledComponent>),
			"assignEnabledComponent", &Sol2Interface::assignEnabledComponent,
			"assignAnimationComponent", &Sol2Interface::assignAnimationComponent,
			"assignParallaxComponent", &Sol2Interface::assignParallaxComponent,
			"assignParticleComponent", &Sol2Interface::assignParticleComponent,
			"assignRenderComponent", &Sol2Interface::assignRenderComponent,
			"assignSBComponent", &Sol2Interface::assignSBComponent,
			"assignTransformComponent", &Sol2Interface::assignTransformComponent,
			"getAnimationComponent", sol::resolve<AnimationComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<AnimationComponent>),
			"getParallaxComponent", sol::resolve<ParallaxComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ParallaxComponent>),
			"getParticleComponent", sol::resolve<ParticleComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ParticleComponent>),
			"getPhysicsComponent", sol::resolve<PhysicsComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<PhysicsComponent>),
			"getRenderComponent", sol::resolve<RenderComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<RenderComponent>),
			"getTransformComponent", sol::resolve<TransformComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<TransformComponent>),
			"getScrollingComponent", sol::resolve<ScrollingBackgroundComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ScrollingBackgroundComponent>)
		);

		m_world->m_lua["registry"] = &(m_world->m_registry);

		// Including state so we can manipulate it from the debug interface and console.
		m_world->m_lua.new_usertype<StateMachine>("StateMachine", 
			"push", &StateMachine::push,
			"pop", &StateMachine::pop);
		m_world->m_lua["stateMachine"] = m_stateMachine.get();

		// AudioPlayer
		m_world->m_lua.new_usertype<MusicPlayer>("MusicPlayer",
			"play", &MusicPlayer::play,
			"stop", &MusicPlayer::stop,
			"resume", &MusicPlayer::resume,
			"changeVolume", &MusicPlayer::changeVolume);
		m_world->m_lua["musicPlayer"] = m_musicPlayer.get();
	}

	Application::~Application()
	{
		// Iterate over entities, destroying their Box2D body data.
		// Yeah, no idea why this has to be done here, entt has some really weird behaviour that's causing a number of unexpected issues.
		Locator::world->m_registry.view<PhysicsComponent>().each([this](entt::DefaultRegistry::entity_type entity, PhysicsComponent& pc) {
			Locator::box2dHelper->m_b2world->DestroyBody(pc.m_body);
		});

		// We want to destroy everything in a specific order to make sure stuff is freed correctly.
		// It actually only really matters box2d is destroyed after the world because the physics code rely
		// on the box2d system to be destroyed unfortunately.
		// And of course the file system being the last to be destroyed.
		m_eventManager.reset();
		m_soundPlayer.reset();
		m_musicPlayer.reset();
		m_shaderLibrary.reset();
		m_fontBook.reset();
		m_textureAtlas.reset();
		m_debugInterface.reset(); 
		m_stateMachine.reset();
		m_box2dHelper.reset();
		m_world.reset();
		m_window.reset();
		m_configReader.reset();
		m_virtualFS.reset();

		// Clean up allegro aswell.
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

	bool Application::run()
	{
		// This is to measure UPS and FPS.
		int frames = 0;
		int updates = 0;
		std::uint64_t timer = 0;
		
		// This is to ensure gameloop is running at 60 UPS, independant of FPS.
		double timePerFrame = 1.0 / 60.0;

		// Set system event sources. User event registration is handled by event manager.
		al_register_event_source(m_eventManager->m_queue, al_get_display_event_source(m_window->getDisplay()));
		al_register_event_source(m_eventManager->m_queue, al_get_mouse_event_source());
		al_register_event_source(m_eventManager->m_queue, al_get_keyboard_event_source());
		
		// Our clock for ensuring gameloop speed.
		ALLEGRO_TIMER* clock = al_create_timer(timePerFrame);
		al_register_event_source(m_eventManager->m_queue, al_get_timer_event_source(clock));
		al_start_timer(clock);
		
		// The timer in milliseconds for UPS and FPS.
		timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		// load the first state
		m_stateMachine->load();

		// Gameloop. Pretty easy to understand.
		// Simply loop the game until the window closes, then the mainloop can handle restarting the application if restart = true.
		while (m_window->isOpen())
		{
			ALLEGRO_EVENT ev;
			while (al_get_next_event(m_eventManager->m_queue, &ev))
			{
				// Events
				m_world->event(&ev);
				m_stateMachine->event(&ev);
				m_debugInterface->event(&ev);

				switch (ev.type)
				{
					case ALLEGRO_EVENT_TIMER:
						// Updates
						m_stateMachine->update(timePerFrame);
						m_world->update(timePerFrame);
						updates++;
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
			
			// We need to "display" the debug ui before the renderer stuff is called.
			// Because this sets up all the textures, api calls, etc.
			m_debugInterface->newFrame();
			m_debugInterface->displayMenu(&m_restart);

			m_window->clear(0, 0, 0);
			
			m_stateMachine->render();
			m_debugInterface->render();

			m_window->display();

			frames++;
			if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - timer) > 1000)
			{
				timer += 1000;
				LOG_S(INFO) << updates << " ups, " << frames << " fps";

				updates = 0;
				frames = 0;
			}
		}

		// unload the last state
		m_stateMachine->unload();

		// Clean up the clock. Everything else is cleaned up by RAII usage.
		al_stop_timer(clock);
		al_destroy_timer(clock);

		return m_restart;
	}
}