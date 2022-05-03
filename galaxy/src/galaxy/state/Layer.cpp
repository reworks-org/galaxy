///
/// Layer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/resource/Resources.hpp"

#include <nlohmann/json.hpp>

#include "Layer.hpp"

namespace galaxy
{
	namespace state
	{
		Layer::Layer(std::string_view name)
			: m_name {name}
			, m_resources {nullptr}
			, m_window {nullptr}
		{
			m_dispatcher.sink<events::MouseWheel>().connect<&graphics::Camera::on_mouse_wheel>(m_camera);
			m_dispatcher.sink<events::WindowResized>().connect<&graphics::Camera::on_window_resized>(m_camera);
			m_window = &core::ServiceLocator<core::Window>::ref();
		}

		Layer::~Layer() noexcept
		{
			m_resources = nullptr;
			m_window    = nullptr;
		}

		void Layer::on_push()
		{
		}

		void Layer::on_pop()
		{
		}

		void Layer::update()
		{
			m_window->trigger_queued_events(m_dispatcher);

			m_camera.update();
			graphics::Renderer::buffer_camera(m_camera);
		}

		void Layer::render()
		{
		}

		void Layer::set_name(std::string_view name) noexcept
		{
			m_name = name;
		}

		void Layer::set_resources(resource::Resources* resources) noexcept
		{
			m_resources = resources;
		}

		const std::string& Layer::get_name() const noexcept
		{
			return m_name;
		}

		nlohmann::json Layer::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["name"]        = m_name;
			json["camera"]      = m_camera.serialize();

			return json;
		}

		void Layer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");
			m_camera.deserialize(json.at("camera"));
		}
	} // namespace state
} // namespace galaxy

/*
		Layer::Layer(std::string_view name) noexcept
			: Serializable {this}
			, m_name {name}
		{
			m_world.create_system<systems::ActionSystem>();
			m_world.create_system<systems::TransformSystem>();
			m_world.create_system<systems::AnimationSystem>();
			m_world.create_system<systems::CollisionSystem>();
			m_world.create_system<systems::ParticleSystem>();
			m_world.create_system<systems::RenderSystem2D>();

			m_rendersystem = m_world.get_system<systems::RenderSystem2D>();
			m_rml          = Rml::CreateContext(std::format("{0}_RmlContext", m_name), {SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height()});

			m_dispatcher.subscribe_callback<events::MouseMoved>([this](const events::MouseMoved& mme) {
				this->m_rml->ProcessMouseMove(static_cast<int>(std::trunc(mme.m_x)), static_cast<int>(std::trunc(mme.m_y)), 0);
			});

			m_dispatcher.subscribe_callback<events::MousePressed>([this](const events::MousePressed& mpe) {
				this->m_rml->ProcessMouseButtonDown(static_cast<int>(mpe.m_button), 0);
			});

			m_dispatcher.subscribe_callback<events::MouseReleased>([this](const events::MouseReleased& mre) {
				this->m_rml->ProcessMouseButtonUp(static_cast<int>(mre.m_button), 0);
			});

			m_dispatcher.subscribe_callback<events::MouseWheel>([this](const events::MouseWheel& mwe) {
				this->m_rml->ProcessMouseWheel(static_cast<float>(mwe.m_y_offset), 0);
			});

			m_dispatcher.subscribe_callback<events::KeyDown>([this](const events::KeyDown& kde) {
				this->m_rml->ProcessKeyDown(ui::RMLInput::handle().m_keymap[kde.m_keycode], ui::RMLInput::handle().m_modmap[kde.m_mod]);
			});

			m_dispatcher.subscribe_callback<events::KeyUp>([this](const events::KeyUp& kue) {
				this->m_rml->ProcessKeyUp(ui::RMLInput::handle().m_keymap[kue.m_keycode], ui::RMLInput::handle().m_modmap[kue.m_mod]);
			});

			m_dispatcher.subscribe_callback<events::KeyChar>([this](const events::KeyChar& kce) {
				if (Rml::Element* element = this->m_rml->GetFocusElement())
				{
					const auto& tag = element->GetTagName();
					if (tag == "input" || tag == "textarea" || tag == "select")
					{
						this->m_rml->ProcessTextInput(kce.m_character);
					}
				}
			});

			m_dispatcher.subscribe_callback<events::WindowResized>([this](const events::WindowResized& wre) {
				this->m_rml->SetDimensions({wre.m_width, wre.m_height});
			});
		}

		Layer::~Layer() noexcept
		{
		}

		void Layer::on_push()
		{
			SL_HANDLE.musicbook()->stop_all();
			SL_HANDLE.lua()->set("galaxy_current_world", &m_world);
		}

		void Layer::on_pop()
		{
			SL_HANDLE.musicbook()->stop_all();
		}

		void Layer::events()
		{
			SL_HANDLE.window()->trigger_queued_events(m_dispatcher);
		}

		void Layer::update()
		{

			m_world.update(this);
			m_rml->Update();
		}

		void Layer::pre_render()
		{
		}

		void Layer::render()
		{

			m_rendersystem->render(m_world);
		}

		void Layer::load_rml_doc(std::string_view document)
		{
			auto* doc = m_rml->LoadDocument(static_cast<std::string>(document));
			if (!doc)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load RML document: {0}.", document);
			}
			else
			{
				doc->Show();
			}
		}

		void Layer::create_maps(std::string_view path)
		{
			m_maps_path = static_cast<std::string>(path);

			m_maps.clear();
			m_maps.load(m_maps_path);
			if (!m_maps.parse(m_world))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse tiled world.");
			}
		}

		void Layer::set_active_map(std::string_view name)
		{
			m_active_map = static_cast<std::string>(name);
		}

		map::Map* Layer::get_map(std::string_view name)
		{
			return m_maps.get_map(name);
		}

		map::Map* Layer::get_active_map()
		{
			return m_maps.get_map(m_active_map);
		}

		nlohmann::json Layer::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["name"]       = m_name;

			json["world"]      = m_world.serialize();
			json["active-map"] = m_active_map;
			json["maps-path"]  = m_maps_path;
			// json["theme"]  = m_gui_theme.serialize();
			// json["gui"]    = m_gui.serialize();

			return json;
		}

		void Layer::deserialize(const nlohmann::json& json)
		{
			m_name = json.at("name");


			m_world.deserialize(json.at("world"));

			m_active_map = json.at("active-map");
			m_maps_path  = json.at("maps-path");

			// m_gui_theme.deserialize(json.at("theme"));
			// m_gui.set_theme(&m_gui_theme);
			// m_gui.deserialize(json.at("gui"));
		}
*/