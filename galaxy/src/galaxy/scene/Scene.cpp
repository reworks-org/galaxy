///
/// Scene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

#include "galaxy/events/GainedFocus.hpp"
#include "galaxy/events/KeyInput.hpp"
#include "galaxy/events/KeyPressed.hpp"
#include "galaxy/events/KeyReleased.hpp"
#include "galaxy/events/LostFocus.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "Scene.hpp"

namespace galaxy
{
	Scene::Scene(const std::string& name) noexcept
		: State {name}
	{
		/*
			 auto& w = core::ServiceLocator<core::Window>::ref();
			 m_camera.set_viewport(w.frame_width(), w.frame_height());
			 auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();
			 m_dispatcher.sink<events::WindowResized>().connect<&Scene::on_window_resized>(this);
			 m_dispatcher.sink<events::MousePressed>().connect<&ui::NuklearUI::on_mouse_pressed>(nui);
			 m_dispatcher.sink<events::MouseWheel>().connect<&ui::NuklearUI::on_mouse_wheel>(nui);
			 m_dispatcher.sink<events::KeyChar>().connect<&ui::NuklearUI::on_key_char>(nui);
			 m_dispatcher.sink<events::KeyPress>().connect<&ui::NuklearUI::on_key_press>(nui);
			 m_dispatcher.sink<events::ContentScale>().connect<&ui::NuklearUI::on_content_scale>(nui);
		*/
	}

	Scene::~Scene() noexcept
	{
	}

	void Scene::on_push()
	{
	}

	void Scene::on_pop()
	{
		if (entt::locator<sol::state>::has_value())
		{
			entt::locator<sol::state>::value().collect_garbage();
		}
	}

	void Scene::on_event(SDL_Event& event)
	{
		// nui.begin_input();
		// nui.end_input();

		/*for (auto& layer : std::views::reverse(m_LayerStack))
		{
			layer->OnEvent(event);
			if (event.Handled)
				break;
		}

		Core::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
		dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) { return OnWindowClosed(e); });

		switch (event.type)
		{
			case SDL_EVENT_SCREEN_KEYBOARD_SHOWN:
			case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN:
				break;

			case SDL_EVENT_LOCALE_CHANGED:
				// TODO: Handle Languages.
				break;

			case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
			case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
			  SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,  /**< Display has changed desktop mode
		//SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,      /**< Display has changed current mode
		//	SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED, /**< Display has changed content scale
			//SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED, /**< Display has changed usable bounds
				// SDL_GetDisplayContentScale
				// SDL_GetWindowDisplayScale
				// https://wiki.libsdl.org/SDL3/README-highdpi
					float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
				break;

			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				// SDL_GetWindowSizeInPixels
				break;

			case SDL_EVENT_WINDOW_RESIZED:
			case SDL_EVENT_WINDOW_MAXIMIZED:
			case SDL_EVENT_WINDOW_RESTORED:
			case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
			case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
			case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED:
				{
					const auto size = window.get_pixel_size();
					dispatcher.enqueue<WindowResized>(size.x, size.y);
				}
				break;

			case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
				// TODO
				// HANDLE RESIZE + DISPLAY SCALE + PIXEL SIZE SCALE.
				break;

			case SDL_EVENT_WINDOW_MOUSE_ENTER:
			case SDL_EVENT_WINDOW_FOCUS_GAINED:
				dispatcher.enqueue<GainedFocus>();
				break;

			case SDL_EVENT_WINDOW_MOUSE_LEAVE:
			case SDL_EVENT_WINDOW_FOCUS_LOST:
				dispatcher.enqueue<LostFocus>();
				break;

			case SDL_EVENT_KEY_DOWN:
				{
					dispatcher.enqueue<KeyPressed>(
						m_events.key.scancode,
						static_cast<Keys>(m_events.key.key),
						static_cast<KeyMods>(m_events.key.mod),
						m_events.key.raw,
						m_events.key.repeat
					);
				}
				break;

			case SDL_EVENT_KEY_UP:
				{
					dispatcher.enqueue<KeyReleased>(
						m_events.key.scancode,
						static_cast<Keys>(m_events.key.key),
						static_cast<KeyMods>(m_events.key.mod),
						m_events.key.raw,
						m_events.key.repeat
					);
				}
				break;

			case SDL_EVENT_TEXT_INPUT:
				// case SDL_EVENT_TEXT_EDITING:
				// case SDL_EVENT_TEXT_EDITING_CANDIDATES:
				{
					auto text = std::string(m_events.text.text);
					dispatcher.enqueue<KeyInput>(std::move(text));
				}
				break;

			case SDL_EVENT_MOUSE_MOTION:
				{
					dispatcher.enqueue<MouseMoved>(m_events.motion.x, m_events.motion.y, m_events.motion.xrel, m_events.motion.yrel);
				}
				break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					dispatcher.enqueue<MousePressed>(m_events.button.x, m_events.button.y, m_events.button.clicks, static_cast<MouseButton>(m_events.button.button));
				}
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					dispatcher.enqueue<MouseReleased>(m_events.button.x, m_events.button.y, m_events.button.clicks, static_cast<MouseButton>(m_events.button.button));
				}
				break;

			case SDL_EVENT_MOUSE_WHEEL:
				{
					dispatcher.enqueue<MouseWheel>(
						m_events.wheel.x,
						m_events.wheel.y,
						m_events.wheel.direction,
						m_events.wheel.mouse_x,
						m_events.wheel.mouse_y,
						m_events.wheel.integer_x,
						m_events.wheel.integer_y
					);
				}
				break;

			case SDL_EVENT_DROP_BEGIN:
				// TODO
				Drag and drop events
				SDL_EVENT_DROP_FILE = 0x1000, /**< The system requests a file open *
				SDL_EVENT_DROP_TEXT,      /**< text/plain drag-and-drop event *
				SDL_EVENT_DROP_BEGIN,     /**< A new set of drops is beginning (NULL filename) *
				SDL_EVENT_DROP_COMPLETE,  /**< Current set of drops is now complete (NULL filename) *
				SDL_EVENT_DROP_POSITION,  /**< Position while moving over the window
		// SDL_DropEvent
		break;

		case SDL_EVENT_JOYSTICK_AXIS_MOTION:
		case SDL_EVENT_JOYSTICK_BALL_MOTION:
		case SDL_EVENT_JOYSTICK_HAT_MOTION:
		case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
		case SDL_EVENT_JOYSTICK_BUTTON_UP:
		case SDL_EVENT_JOYSTICK_REMOVED:
		case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
		case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
			// TODO
			break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		case SDL_EVENT_GAMEPAD_REMOVED:
		case SDL_EVENT_GAMEPAD_REMAPPED:
		case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
		case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
		case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
		case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
		case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
		case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
			// TODO
			break;

		case SDL_EVENT_FINGER_DOWN:
		case SDL_EVENT_FINGER_UP:
		case SDL_EVENT_FINGER_MOTION:
		case SDL_EVENT_FINGER_CANCELED:
			// TODO
			break;

		case SDL_EVENT_PEN_PROXIMITY_IN:
		case SDL_EVENT_PEN_PROXIMITY_OUT:
		case SDL_EVENT_PEN_DOWN:
		case SDL_EVENT_PEN_UP:
		case SDL_EVENT_PEN_BUTTON_DOWN:
		case SDL_EVENT_PEN_BUTTON_UP:
		case SDL_EVENT_PEN_MOTION:
		case SDL_EVENT_PEN_AXIS:
			// TODO
			break;

		default:
			break;
	}
	*/
	}

	void Scene::update(EntityManager& em)
	{
		// make sure events are propagated from top to bottom.
		// make sure updting is from btm to top.

		// graphics::Renderer::ref().flush();
		// m_registry.update(m_b2world);
		// world.update();
		// m_b2world.Step(GALAXY_DT, m_velocity_iterations, m_position_iterations);
		m_sysman.update(em, this);
	}

	void Scene::render()
	{
		// render scene
		// then any ui layers.

		/*
			graphics::Renderer::ref().begin_post();

			// Scene specific.
			graphics::Renderer::ref().submit_camera(m_camera);
			graphics::Renderer::ref().draw();

			graphics::Renderer::ref().end_post();
			graphics::Renderer::ref().begin_default();
			graphics::Renderer::ref().render_post();
			graphics::Renderer::ref().end_default();

			// Scene specific.
			auto& nui = core::ServiceLocator<ui::NuklearUI>::ref();

			nui.new_frame();
			update_ui();
			nui.render();

			graphics::Renderer::ref().end_default();
			*/
	}

	nlohmann::json Scene::serialize()
	{
		nlohmann::json json = "{}"_json;

		/*json["camera"]  = m_camera.serialize();
		json["physics"] = nlohmann::json::object();
		auto& physics   = json.at("physics");

		auto gravity            = m_b2world.GetGravity();
		physics["gravity"]["x"] = gravity.x;
		physics["gravity"]["y"] = gravity.y;

		physics["allow_sleeping"]        = m_b2world.GetAllowSleeping();
		physics["allow_autoclearforces"] = m_b2world.GetAutoClearForces();
		physics["velocity_iterations"]   = m_velocity_iterations;
		physics["position_iterations"]   = m_position_iterations;
		json["name"]                     = m_name;
		json["ldtk_world"]               = m_world.file();
		json["current_map"]              = m_world.get_active() ? m_world.get_active()->name() : "";

		json["camera"]                   = m_camera.serialize();
		json["entities"]                 = nlohmann::json::array();

		auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

		for (const auto& [entity] : m_registry.m_entt.view<entt::entity>(entt::exclude<flags::DenySerialization>).each())
		{
			json["entities"].push_back(em.serialize_entity(entity, m_registry.m_entt));
		}

		json["physics"] = nlohmann::json::object();
		auto& physics   = json.at("physics");

		auto gravity            = m_b2world.GetGravity();
		physics["gravity"]["x"] = gravity.x;
		physics["gravity"]["y"] = gravity.y;

		physics["allow_sleeping"]        = m_b2world.GetAllowSleeping();
		physics["allow_autoclearforces"] = m_b2world.GetAutoClearForces();
		physics["velocity_iterations"]   = m_velocity_iterations;
		physics["position_iterations"]   = m_position_iterations;
		json["name"]                     = m_name;
		json["ldtk_world"]               = m_world.file();
		json["current_map"]              = m_world.get_active() ? m_world.get_active()->name() : "";

		nlohmann::json json = "{}"_json;
		json["name"]        = m_name;

		json["systems"] = nlohmann::json::object();
		for (auto i = 0; i < m_systems.size(); i++)
		{
			json["systems"][std::to_string(i)] = m_systems[i]->id();
		}*/

		return json;
	}

	void Scene::deserialize(const nlohmann::json& json)
	{
		/*m_name = json.at("name");

		const auto& systems = json.at("systems");
		m_systems.reserve(systems.size());
		for (const auto& [index, name] : systems.items())
		{
			add_system(name.get<std::string>());
		}

		m_camera.deserialize(json.at("camera"));
		auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

		const auto& physics = json.at("physics");
		const auto& gravity = physics.at("gravity");

		m_b2world.SetGravity({gravity.at("x"), gravity.at("y")});
		m_b2world.SetAllowSleeping(physics.at("allow_sleeping"));
		m_b2world.SetAutoClearForces(physics.at("allow_autoclearforces"));
		m_velocity_iterations = physics.at("velocity_iterations");
		m_position_iterations = physics.at("position_iterations");

		if (load_world(json.at("ldtk_world")))
		{
			m_world.set_active(json.at("current_map"));
		}
		m_camera.deserialize(json.at("camera"));
		auto& em = core::ServiceLocator<meta::EntityMeta>::ref();

		m_registry.clear();

		const auto& physics = json.at("physics");
		const auto& gravity = physics.at("gravity");

		m_b2world.SetGravity({gravity.at("x"), gravity.at("y")});
		m_b2world.SetAllowSleeping(physics.at("allow_sleeping"));
		m_b2world.SetAutoClearForces(physics.at("allow_autoclearforces"));
		m_velocity_iterations = physics.at("velocity_iterations");
		m_position_iterations = physics.at("position_iterations");

		const auto& entity_json = json.at("entities");
		for (const auto& data : entity_json)
		{
			const auto entity = em.deserialize_entity(data, m_registry.m_entt);

			if (!m_registry.m_entt.all_of<components::Tag>(entity))
			{
				auto& tag = m_registry.m_entt.emplace<components::Tag>(entity);
				tag.m_tag = "Untagged";
			}
		}

		m_name = json.at("name");

		if (load_world(json.at("ldtk_world")))
		{
			m_world.set_active(json.at("current_map"));
		}*/
	}

	SystemManager& Scene::sys_man() noexcept
	{
		return m_sysman;
	}

	/*void Scene::update_ui()
		{
			const auto view = m_registry.m_entt.view<components::GUI>(entt::exclude<flags::Disabled>);
			for (auto&& [entity, gui] : view.each())
			{
				if (gui.m_update.valid())
				{
					gui.m_update(gui.m_self);
				}
			}
		}
		bool Scene::load_world(const std::string& file)
		{
			if (m_world.load(file))
			{
				m_world.parse();
				return true;
			}

			return false;
		}



	*/
} // namespace galaxy
