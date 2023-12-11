///
/// LuaGalaxy.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/MapData.hpp"
#include "galaxy/components/ParticleGenerator.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/components/UIScript.hpp"
#include "galaxy/core/Config.hpp"
#include "galaxy/core/Loader.hpp"
#include "galaxy/core/Prefab.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/map/Map.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/FNV1a.hpp"
#include "galaxy/math/Generic.hpp"
#include "galaxy/math/Random.hpp"
#include "galaxy/math/Rect.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/media/Sound.hpp"
#include "galaxy/media/Video.hpp"
#include "galaxy/meta/EntityMeta.hpp"
#include "galaxy/platform/Subprocess.hpp"
#include "galaxy/resource/BasicScripts.hpp"
#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Media.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/SceneManager.hpp"
#include "galaxy/state/StateMachine.hpp"
#include "galaxy/ui/NuklearUI.hpp"
#include "galaxy/utils/Guid.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void log_wrapper(error::LogLevel error_level, std::string_view message)
		{
			switch (error_level)
			{
				case error::LogLevel::INFO:
					GALAXY_LOG(GALAXY_INFO, "{0}", message);
					break;

				case error::LogLevel::DEBUG:
					GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
					break;

				case error::LogLevel::WARNING:
					GALAXY_LOG(GALAXY_WARNING, "{0}", message);
					break;

				case error::LogLevel::ERROR:
					GALAXY_LOG(GALAXY_ERROR, "{0}", message);
					break;

				case error::LogLevel::FATAL:
					GALAXY_LOG(GALAXY_FATAL, "{0}", message);
					break;
			}
		}

		void load_config_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_user_config();
		}

		void load_window_wrapper()
		{
			core::ServiceLocator<core::Loader>::ref().load_window();
		}

		void inject_galaxy()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.set_function("galaxy_load_user_config", &load_config_wrapper);
			lua.set_function("galaxy_load_window_config", &load_window_wrapper);

			/* COMPONENTS */
			auto animated_type = lua.new_usertype<components::Animated>("Animated",
				sol::constructors<components::Animated()>(),
				"type_id",
				&entt::type_hash<components::Animated>::value);

			animated_type["active"]              = &components::Animated::active;
			animated_type["add"]                 = &components::Animated::add;
			animated_type["is_paused"]           = &components::Animated::is_paused;
			animated_type["animations"]          = &components::Animated::m_animations;
			animated_type["time_spent_on_frame"] = &components::Animated::m_time_spent_on_frame;
			animated_type["pause"]               = &components::Animated::pause;
			animated_type["play"]                = sol::resolve<void(void)>(&components::Animated::play);
			animated_type["set_and_play"]        = sol::resolve<void(const std::string&)>(&components::Animated::play);
			animated_type["set"]                 = &components::Animated::set;
			animated_type["stop"]                = &components::Animated::stop;

			auto mapdata_type = lua.new_usertype<components::MapData>("MapData",
				sol::constructors<components::MapData()>(),
				"type_id",
				&entt::type_hash<components::MapData>::value);

			mapdata_type["set_texture"] = &components::MapData::set_texture;

			auto pg_type = lua.new_usertype<components::ParticleGenerator>("ParticleGenerator",
				sol::constructors<components::ParticleGenerator()>(),
				"type_id",
				&entt::type_hash<components::ParticleGenerator>::value);

			pg_type["generate"]                = &components::ParticleGenerator::generate;
			pg_type["regenerate"]              = &components::ParticleGenerator::regenerate;
			pg_type["count"]                   = &components::ParticleGenerator::m_count;
			pg_type["fixed_alpha"]             = &components::ParticleGenerator::m_fixed_alpha;
			pg_type["fixed_colour"]            = &components::ParticleGenerator::m_fixed_colour;
			pg_type["fixed_life"]              = &components::ParticleGenerator::m_fixed_life;
			pg_type["fixed_scale"]             = &components::ParticleGenerator::m_fixed_scale;
			pg_type["fixed_vel"]               = &components::ParticleGenerator::m_fixed_vel;
			pg_type["keep_scale_aspect_ratio"] = &components::ParticleGenerator::m_keep_scale_aspect_ratio;
			pg_type["max_alpha"]               = &components::ParticleGenerator::m_max_alpha;
			pg_type["max_colour"]              = &components::ParticleGenerator::m_max_colour;
			pg_type["max_life"]                = &components::ParticleGenerator::m_max_life;
			pg_type["max_rect_spread"]         = &components::ParticleGenerator::m_max_rect_spread;
			pg_type["max_scale"]               = &components::ParticleGenerator::m_max_scale;
			pg_type["max_vel"]                 = &components::ParticleGenerator::m_max_vel;
			pg_type["min_alpha"]               = &components::ParticleGenerator::m_min_alpha;
			pg_type["min_colour"]              = &components::ParticleGenerator::m_min_colour;
			pg_type["min_life"]                = &components::ParticleGenerator::m_min_life;
			pg_type["min_rect_spread"]         = &components::ParticleGenerator::m_min_rect_spread;
			pg_type["min_scale"]               = &components::ParticleGenerator::m_min_scale;
			pg_type["min_vel"]                 = &components::ParticleGenerator::m_min_vel;
			pg_type["randomize_colour"]        = &components::ParticleGenerator::m_randomize_colour;
			pg_type["randomize_colour_alpha"]  = &components::ParticleGenerator::m_randomize_colour_alpha;
			pg_type["randomize_initial_vel"]   = &components::ParticleGenerator::m_randomize_initial_vel;
			pg_type["randomize_life"]          = &components::ParticleGenerator::m_randomize_life;
			pg_type["randomize_position"]      = &components::ParticleGenerator::m_randomize_position;
			pg_type["randomize_scale"]         = &components::ParticleGenerator::m_randomize_scale;
			pg_type["spread"]                  = &components::ParticleGenerator::m_spread;
			pg_type["spread_radius"]           = &components::ParticleGenerator::m_spread_radius;
			pg_type["layer"]                   = &components::ParticleGenerator::m_layer;
			pg_type["texture"]                 = &components::ParticleGenerator::m_texture;
			pg_type["start_pos"]               = &components::ParticleGenerator::m_start_pos;
			pg_type["reset"]                   = sol::resolve<void(void)>(&components::ParticleGenerator::reset);

			auto primitive_data_type =
				lua.new_usertype<components::Primitive::PrimitiveData>("PrimitiveData", sol::constructors<components::Primitive::PrimitiveData()>());
			primitive_data_type["fragments"] = &components::Primitive::PrimitiveData::fragments;
			primitive_data_type["points"]    = &components::Primitive::PrimitiveData::points;
			primitive_data_type["radii"]     = &components::Primitive::PrimitiveData::radii;
			primitive_data_type["radius"]    = &components::Primitive::PrimitiveData::radius;
			primitive_data_type["start_end"] = &components::Primitive::PrimitiveData::start_end;

			auto primitive_type = lua.new_usertype<components::Primitive>("Primitive",
				sol::constructors<components::Primitive()>(),
				"type_id",
				&entt::type_hash<components::Primitive>::value);

			primitive_type["create_circle"]   = &components::Primitive::create<math::Shape::CIRCLE>;
			primitive_type["create_ellipse"]  = &components::Primitive::create<math::Shape::ELLIPSE>;
			primitive_type["create_line"]     = &components::Primitive::create<math::Shape::LINE>;
			primitive_type["create_point"]    = &components::Primitive::create<math::Shape::POINT>;
			primitive_type["create_polygon"]  = &components::Primitive::create<math::Shape::POLYGON>;
			primitive_type["create_polyline"] = &components::Primitive::create<math::Shape::POLYLINE>;
			primitive_type["get_data"]        = &components::Primitive::get_data;
			primitive_type["get_height"]      = &components::Primitive::get_height;
			primitive_type["get_shape"]       = &components::Primitive::get_shape;
			primitive_type["get_width"]       = &components::Primitive::get_width;
			primitive_type["colour"]          = &components::Primitive::m_colour;

			auto rigidbody_type = lua.new_usertype<components::RigidBody>("RigidBody",
				sol::constructors<components::RigidBody()>(),
				"type_id",
				&entt::type_hash<components::RigidBody>::value);

			rigidbody_type["get_density"]               = &components::RigidBody::get_density;
			rigidbody_type["get_friction"]              = &components::RigidBody::get_friction;
			rigidbody_type["get_restitution"]           = &components::RigidBody::get_restitution;
			rigidbody_type["get_restitution_threshold"] = &components::RigidBody::get_restitution_threshold;
			rigidbody_type["get_shape"]                 = &components::RigidBody::get_shape;
			rigidbody_type["get_type"]                  = &components::RigidBody::get_type;
			rigidbody_type["is_bullet"]                 = &components::RigidBody::is_bullet;
			rigidbody_type["is_rotation_fixed"]         = &components::RigidBody::is_rotation_fixed;
			rigidbody_type["set_bullet"]                = &components::RigidBody::set_bullet;
			rigidbody_type["set_density"]               = &components::RigidBody::set_density;
			rigidbody_type["set_fixed_rotation"]        = &components::RigidBody::set_fixed_rotation;
			rigidbody_type["set_friction"]              = &components::RigidBody::set_friction;
			rigidbody_type["set_restitution"]           = &components::RigidBody::set_restitution;
			rigidbody_type["set_restitution_threshold"] = &components::RigidBody::set_restitution_threshold;
			rigidbody_type["set_shape"]                 = &components::RigidBody::set_shape;
			rigidbody_type["set_type"]                  = &components::RigidBody::set_type;

			auto script_type      = lua.new_usertype<components::Script>("Script",
                sol::constructors<components::Script()>(),
                "type_id",
                &entt::type_hash<components::Script>::value);
			script_type["file"]   = &components::Script::file;
			script_type["self"]   = &components::Script::m_self;
			script_type["update"] = &components::Script::m_update;

			auto sprite_type = lua.new_usertype<components::Sprite>("Sprite",
				sol::constructors<components::Sprite()>(),
				"type_id",
				&entt::type_hash<components::Sprite>::value);

			sprite_type["create"]             = sol::resolve<void(const std::string&, const int, const float)>(&components::Sprite::create);
			sprite_type["create_texturerect"] = sol::resolve<void(const std::string&, const math::iRect&, const int, const float)>(&components::Sprite::create);
			sprite_type["update"]             = sol::resolve<void(const std::string&)>(&components::Sprite::update);
			sprite_type["update_texturerect"] = sol::resolve<void(const std::string&, const math::iRect&)>(&components::Sprite::update);
			sprite_type["get_height"]         = &components::Sprite::get_height;
			sprite_type["get_opacity"]        = &components::Sprite::get_opacity;
			sprite_type["get_width"]          = &components::Sprite::get_width;
			sprite_type["set_opacity"]        = &components::Sprite::set_opacity;

			auto tag_type =
				lua.new_usertype<components::Tag>("Tag", sol::constructors<components::Tag()>(), "type_id", &entt::type_hash<components::Tag>::value);
			tag_type["tag"] = &components::Tag::m_tag;

			auto uiscript_type    = lua.new_usertype<components::UIScript>("UIScript",
                sol::constructors<components::UIScript()>(),
                "type_id",
                &entt::type_hash<components::UIScript>::value);
			uiscript_type["file"] = &components::UIScript::file;

			// clang-format off
			lua.new_enum<components::Text::Alignment>("TextAlignment",
			{
				{"LEFT", components::Text::Alignment::LEFT},
				{"CENTER", components::Text::Alignment::CENTER},
				{"RIGHT", components::Text::Alignment::RIGHT}
			});
			// clang-format on

			auto text_type =
				lua.new_usertype<components::Text>("Text", sol::constructors<components::Text()>(), "type_id", &entt::type_hash<components::Text>::value);
			text_type["create"]        = &components::Text::create;
			text_type["get_height"]    = &components::Text::get_height;
			text_type["get_size"]      = &components::Text::get_size;
			text_type["get_text"]      = &components::Text::get_text;
			text_type["get_width"]     = &components::Text::get_width;
			text_type["get_width"]     = &components::Text::get_font;
			text_type["get_alignment"] = &components::Text::get_alignment;
			text_type["colour"]        = &components::Text::m_colour;

			text_type["update_text"]             = sol::resolve<void(std::string_view)>(&components::Text::update);
			text_type["update_text_size"]        = sol::resolve<void(std::string_view, const float)>(&components::Text::update);
			text_type["update_text_colour"]      = sol::resolve<void(std::string_view, const graphics::Colour&)>(&components::Text::update);
			text_type["update_text_size_colour"] = sol::resolve<void(std::string_view, const float, const graphics::Colour&)>(&components::Text::update);
			text_type["update_text_alignment"]   = sol::resolve<void(std::string_view, const components::Text::Alignment)>(&components::Text::update);
			text_type["update_text_size_alignment"] =
				sol::resolve<void(std::string_view, const float, const components::Text::Alignment)>(&components::Text::update);
			text_type["update_text_colour_alignment"] =
				sol::resolve<void(std::string_view, const graphics::Colour&, const components::Text::Alignment)>(&components::Text::update);
			text_type["update_text_size_colour_alignment"] =
				sol::resolve<void(std::string_view, const float, const graphics::Colour&, const components::Text::Alignment)>(&components::Text::update);

			auto transform_type = lua.new_usertype<components::Transform>("Transform",
				sol::constructors<components::Transform()>(),
				"type_id",
				&entt::type_hash<components::Transform>::value);

			transform_type["get_origin"]           = &components::Transform::get_origin;
			transform_type["get_pos"]              = &components::Transform::get_pos;
			transform_type["get_rotation"]         = &components::Transform::get_rotation;
			transform_type["get_scale"]            = &components::Transform::get_scale;
			transform_type["get_transform"]        = &components::Transform::get_transform;
			transform_type["reset"]                = &components::Transform::reset;
			transform_type["rotate"]               = &components::Transform::rotate;
			transform_type["set_scale"]            = &components::Transform::set_scale;
			transform_type["set_scale_horizontal"] = &components::Transform::set_scale_horizontal;
			transform_type["set_scale_vertical"]   = &components::Transform::set_scale_vertical;
			transform_type["set_origin"]           = &components::Transform::set_origin;
			transform_type["set_pos"]              = &components::Transform::set_pos;
			transform_type["set_rotation"]         = &components::Transform::set_rotation;
			transform_type["translate"]            = &components::Transform::translate;

			/* CORE */
			auto prefab_type =
				lua.new_usertype<core::Prefab>("Prefab", sol::constructors<core::Prefab(entt::entity, entt::registry&), core::Prefab(const nlohmann::json&)>());
			prefab_type["from_entity"] = &core::Prefab::from_entity;
			prefab_type["from_json"]   = &core::Prefab::from_json;
			prefab_type["to_entity"]   = &core::Prefab::to_entity;
			prefab_type["to_json"]     = &core::Prefab::to_json;

			auto config_type                 = lua.new_usertype<core::Config>("Config", sol::no_constructor);
			config_type["save"]              = &core::Config::save;
			config_type["has_at_root"]       = sol::resolve<bool(const std::string&)>(&core::Config::has);
			config_type["has_in_section"]    = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::has);
			config_type["get_bool"]          = sol::resolve<bool(const std::string&)>(&core::Config::get<bool>);
			config_type["get_int"]           = sol::resolve<int(const std::string&)>(&core::Config::get<int>);
			config_type["get_float"]         = sol::resolve<float(const std::string&)>(&core::Config::get<float>);
			config_type["get_string"]        = sol::resolve<std::string(const std::string&)>(&core::Config::get<std::string>);
			config_type["get_section_bool"]  = sol::resolve<bool(const std::string&, const std::string&, const std::string&)>(&core::Config::get<bool>);
			config_type["get_section_int"]   = sol::resolve<int(const std::string&, const std::string&, const std::string&)>(&core::Config::get<int>);
			config_type["get_section_float"] = sol::resolve<float(const std::string&, const std::string&, const std::string&)>(&core::Config::get<float>);
			config_type["get_section_string"] =
				sol::resolve<std::string(const std::string&, const std::string&, const std::string&)>(&core::Config::get<std::string>);

			// Cannot be created in lua, accessed from scene instead.
			auto world_type                   = lua.new_usertype<core::World>("World", sol::no_constructor);
			world_type["clear"]               = &core::World::clear;
			world_type["registry"]            = &core::World::m_registry;
			world_type["create"]              = &core::World::create;
			world_type["create_from_prefab"]  = &core::World::create_from_prefab;
			world_type["is_valid"]            = &core::World::is_valid;
			world_type["velocity_iterations"] = &core::World::m_velocity_iterations;
			world_type["position_iterations"] = &core::World::m_position_iterations;

			/* ERROR */
			// clang-format off
			lua.new_enum<error::LogLevel>("LogLevels",
			{
				{"INFO", error::LogLevel::INFO},
				{"DEBUG", error::LogLevel::DEBUG},
				{"WARNING", error::LogLevel::WARNING},
				{"ERROR", error::LogLevel::ERROR},
				{"FATAL", error::LogLevel::FATAL}
			});
			// clang-format on

			lua.set_function("galaxy_log", &log_wrapper);

			/* EVENTS */
			auto contentscale_type       = lua.new_usertype<events::ContentScale>("ContentScale", sol::constructors<events::ContentScale()>());
			contentscale_type["type_id"] = &entt::type_hash<events::ContentScale>::value;
			contentscale_type["xscale"]  = &events::ContentScale::xscale;
			contentscale_type["yscale"]  = &events::ContentScale::yscale;

			auto keychar_type         = lua.new_usertype<events::KeyChar>("KeyChar", sol::constructors<events::KeyChar()>());
			keychar_type["type_id"]   = &entt::type_hash<events::KeyChar>::value;
			keychar_type["codepoint"] = &events::KeyChar::codepoint;
			keychar_type["handled"]   = &events::KeyChar::handled;

			auto keydown_type        = lua.new_usertype<events::KeyDown>("KeyDown", sol::constructors<events::KeyDown()>());
			keydown_type["type_id"]  = &entt::type_hash<events::KeyDown>::value;
			keydown_type["keycode"]  = &events::KeyDown::keycode;
			keydown_type["mod"]      = &events::KeyDown::mod;
			keydown_type["scancode"] = &events::KeyDown::scancode;
			keydown_type["handled"]  = &events::KeyDown::handled;

			auto keyrepeat_type        = lua.new_usertype<events::KeyRepeat>("KeyRepeat", sol::constructors<events::KeyRepeat()>());
			keyrepeat_type["type_id"]  = &entt::type_hash<events::KeyRepeat>::value;
			keyrepeat_type["keycode"]  = &events::KeyRepeat::keycode;
			keyrepeat_type["mod"]      = &events::KeyRepeat::mod;
			keyrepeat_type["scancode"] = &events::KeyRepeat::scancode;
			keyrepeat_type["handled"]  = &events::KeyRepeat::handled;

			auto keyup_type        = lua.new_usertype<events::KeyUp>("KeyUp", sol::constructors<events::KeyUp()>());
			keyup_type["type_id"]  = &entt::type_hash<events::KeyUp>::value;
			keyup_type["keycode"]  = &events::KeyUp::keycode;
			keyup_type["mod"]      = &events::KeyUp::mod;
			keyup_type["scancode"] = &events::KeyUp::scancode;
			keyup_type["handled"]  = &events::KeyUp::handled;

			auto mouseentered_type       = lua.new_usertype<events::MouseEnter>("MouseEnter", sol::constructors<events::MouseEnter()>());
			mouseentered_type["type_id"] = &entt::type_hash<events::MouseEnter>::value;
			mouseentered_type["entered"] = &events::MouseEnter::entered;
			mouseentered_type["handled"] = &events::MouseEnter::handled;

			auto mousemoved_type       = lua.new_usertype<events::MouseMoved>("MouseMoved", sol::constructors<events::MouseMoved()>());
			mousemoved_type["type_id"] = &entt::type_hash<events::MouseMoved>::value;
			mousemoved_type["xpos"]    = &events::MouseMoved::xpos;
			mousemoved_type["ypos"]    = &events::MouseMoved::ypos;
			mousemoved_type["handled"] = &events::MouseMoved::handled;

			auto mousepressed_type       = lua.new_usertype<events::MousePressed>("MousePressed", sol::constructors<events::MousePressed()>());
			mousepressed_type["type_id"] = &entt::type_hash<events::MousePressed>::value;
			mousepressed_type["button"]  = &events::MousePressed::button;
			mousepressed_type["mod"]     = &events::MousePressed::mod;
			mousepressed_type["xpos"]    = &events::MousePressed::xpos;
			mousepressed_type["ypos"]    = &events::MousePressed::ypos;
			mousepressed_type["handled"] = &events::MousePressed::handled;

			auto mousereleased_type       = lua.new_usertype<events::MouseReleased>("MouseReleased", sol::constructors<events::MouseReleased()>());
			mousereleased_type["type_id"] = &entt::type_hash<events::MouseReleased>::value;
			mousereleased_type["button"]  = &events::MouseReleased::button;
			mousereleased_type["mod"]     = &events::MouseReleased::mod;
			mousereleased_type["xpos"]    = &events::MouseReleased::xpos;
			mousereleased_type["ypos"]    = &events::MouseReleased::ypos;
			mousereleased_type["handled"] = &events::MouseReleased::handled;

			auto mousewheel_type       = lua.new_usertype<events::MouseWheel>("MouseWheel", sol::constructors<events::MouseWheel()>());
			mousewheel_type["type_id"] = &entt::type_hash<events::MouseWheel>::value;
			mousewheel_type["xoff"]    = &events::MouseWheel::xoff;
			mousewheel_type["yoff"]    = &events::MouseWheel::yoff;
			mousewheel_type["handled"] = &events::MouseWheel::handled;

			auto windowresized_type       = lua.new_usertype<events::WindowResized>("WindowResized", sol::constructors<events::WindowResized()>());
			windowresized_type["type_id"] = &entt::type_hash<events::WindowResized>::value;
			windowresized_type["width"]   = &events::WindowResized::width;
			windowresized_type["height"]  = &events::WindowResized::height;

			auto windowclosed_type       = lua.new_usertype<events::WindowClosed>("WindowClosed", sol::constructors<events::WindowClosed()>());
			windowclosed_type["type_id"] = &entt::type_hash<events::WindowClosed>::value;

			/* FLAGS */
			lua.new_usertype<flags::DenySerialization>("DenySerialization",
				sol::constructors<flags::DenySerialization()>(),
				"type_id",
				&entt::type_hash<flags::DenySerialization>::value);

			lua.new_usertype<flags::Disabled>("Disabled", sol::constructors<flags::Disabled()>(), "type_id", &entt::type_hash<flags::Disabled>::value);

			/* FS */
			// clang-format off
			lua.new_enum<fs::AssetType>("AssetType",
			{
				{"ATLAS", fs::AssetType::ATLAS},
				{"FONT", fs::AssetType::FONT},
				{"LANG", fs::AssetType::LANG},
				{"MAPS", fs::AssetType::MAPS},
				{"MUSIC", fs::AssetType::MUSIC},
				{"PREFABS", fs::AssetType::PREFABS},
				{"SCRIPT", fs::AssetType::SCRIPT},
				{"SFX", fs::AssetType::SFX},
				{"SHADER", fs::AssetType::SHADER},
				{"TEXTURE", fs::AssetType::TEXTURE},
				{"UI", fs::AssetType::UI},
				{"UI_FONT", fs::AssetType::UI_FONT},
				{"UNKNOWN", fs::AssetType::UNKNOWN},
				{"VIDEO", fs::AssetType::VIDEO},
				{"VOICE", fs::AssetType::VOICE}
			});

			lua.new_enum<fs::DialogButton>("DialogButton",
            {
				{"cancel_no", fs::DialogButton::cancel_no},
				{"ok_yes", fs::DialogButton::ok_yes},
				{"yes_no_cancel", fs::DialogButton::yes_no_cancel}
            });

			lua.new_enum<fs::DialogIcon>("DialogIcons",
			{
                {"info", fs::DialogIcon::info},
				{"warning", fs::DialogIcon::warning},
				{"error", fs::DialogIcon::error}
            });

			lua.new_enum<fs::DialogType>("DialogType",
			{
			    {"ok", fs::DialogType::ok},
				{"okcancel", fs::DialogType::okcancel},
				{"yesno", fs::DialogType::yesno},
				{"yesnocancel", fs::DialogType::yesnocancel}
			});
			// clang-format on

			auto archiveentry_type     = lua.new_usertype<fs::ArchiveEntry>("ArchiveEntry", sol::constructors<fs::ArchiveEntry()>());
			archiveentry_type["pack"]  = &fs::ArchiveEntry::pack;
			archiveentry_type["index"] = &fs::ArchiveEntry::index;
			archiveentry_type["type"]  = &fs::ArchiveEntry::type;

			auto vfs_type                    = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["alert"]                = &fs::VirtualFileSystem::alert;
			vfs_type["find"]                 = &fs::VirtualFileSystem::find;
			vfs_type["open_file_dialog"]     = &fs::VirtualFileSystem::open_file_dialog;
			vfs_type["open_save_dialog"]     = &fs::VirtualFileSystem::open_save_dialog;
			vfs_type["select_folder_dialog"] = &fs::VirtualFileSystem::select_folder_dialog;
			vfs_type["contains"]             = &fs::VirtualFileSystem::contains;
			vfs_type["import"]               = &fs::VirtualFileSystem::import;
			vfs_type["input_box"]            = &fs::VirtualFileSystem::input_box;
			vfs_type["list_assets"]          = &fs::VirtualFileSystem::list_assets;
			vfs_type["message_box"]          = &fs::VirtualFileSystem::message_box;
			vfs_type["mkdir_disk"]           = &fs::VirtualFileSystem::mkdir_disk;
			vfs_type["notification"]         = &fs::VirtualFileSystem::notification;
			vfs_type["rebuild_filesystem"]   = &fs::VirtualFileSystem::rebuild_filesystem;

			/* GRAPHICS */
			// clang-format off
			lua.new_enum<graphics::Primitives>("Primitives",
			{
				{"LINE", graphics::Primitives::LINE},
				{"LINE_LOOP", graphics::Primitives::LINE_LOOP},
				{"LINE_STRIP", graphics::Primitives::LINE_STRIP},
				{"POINT", graphics::Primitives::POINT},
				{"TRIANGLE", graphics::Primitives::TRIANGLE},
				{"TRIANGLE_FAN", graphics::Primitives::TRIANGLE_FAN},
				{"TRIANGLE_STRIP", graphics::Primitives::TRIANGLE_STRIP}
			});
			// clang-format on

			// clang-format off
			lua.new_enum<math::Shape>("Shape",
			{
				{"CIRCLE", math::Shape::CIRCLE},
				{"ELLIPSE", math::Shape::ELLIPSE},
				{"LINE", math::Shape::LINE},
				{"POINT", math::Shape::POINT},
				{"POLYGON", math::Shape::POLYGON},
				{"POLYLINE", math::Shape::POLYLINE}
			});
			// clang-format on

			auto colour_type        = lua.new_usertype<graphics::Colour>("Colour",
                sol::constructors<graphics::Colour(), graphics::Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
			colour_type["alpha"]    = &graphics::Colour::m_alpha;
			colour_type["blue"]     = &graphics::Colour::m_blue;
			colour_type["green"]    = &graphics::Colour::m_green;
			colour_type["red"]      = &graphics::Colour::m_red;
			colour_type["to_array"] = &graphics::Colour::to_array;
			colour_type["to_vec4"]  = &graphics::Colour::to_vec4;

			auto uniform_type        = lua.new_usertype<graphics::UniformInfo>("UniformInfo", sol::constructors<graphics::UniformInfo>());
			uniform_type["count"]    = &graphics::UniformInfo::count;
			uniform_type["location"] = &graphics::UniformInfo::location;

			auto shader_type =
				lua.new_usertype<graphics::Shader>("Shader", sol::constructors<graphics::Shader(), graphics::Shader(const std::string&, const std::string&)>());
			shader_type["id"]                   = &graphics::Shader::id;
			shader_type["get_uniform_count"]    = &graphics::Shader::get_uniform_count;
			shader_type["get_uniform_info"]     = &graphics::Shader::get_uniform_info;
			shader_type["get_uniform_location"] = &graphics::Shader::get_uniform_location;
			shader_type["compile"]              = &graphics::Shader::compile;
			shader_type["destroy"]              = &graphics::Shader::destroy;
			shader_type["set_uniform_int"]      = &graphics::Shader::set_uniform<int>;
			shader_type["set_uniform_int2"]     = &graphics::Shader::set_uniform<int, int>;
			shader_type["set_uniform_int3"]     = &graphics::Shader::set_uniform<int, int, int>;
			shader_type["set_uniform_int4"]     = &graphics::Shader::set_uniform<int, int, int, int>;
			shader_type["set_uniform_float"]    = &graphics::Shader::set_uniform<float>;
			shader_type["set_uniform_float2"]   = &graphics::Shader::set_uniform<float, float>;
			shader_type["set_uniform_float3"]   = &graphics::Shader::set_uniform<float, float, float>;
			shader_type["set_uniform_float4"]   = &graphics::Shader::set_uniform<float, float, float, float>;
			shader_type["set_uniform_bool"]     = &graphics::Shader::set_uniform<bool>;
			shader_type["set_uniform_colour"]   = &graphics::Shader::set_uniform<graphics::Colour>;
			shader_type["load"]                 = &graphics::Shader::load;
			shader_type["load_raw"]             = sol::resolve<bool(const std::string&)>(&graphics::Shader::load_raw);
			shader_type["load_raw_separate"]    = sol::resolve<bool(const std::string&, const std::string&)>(&graphics::Shader::load_raw);

			auto camera_type                 = lua.new_usertype<graphics::Camera>("Camera", sol::constructors<graphics::Camera(bool)>());
			camera_type["set_viewport"]      = &graphics::Camera::set_viewport;
			camera_type["get_viewport"]      = &graphics::Camera::get_viewport;
			camera_type["set_pos"]           = &graphics::Camera::set_pos;
			camera_type["set_rotation"]      = &graphics::Camera::set_rotation;
			camera_type["get_x"]             = &graphics::Camera::get_x;
			camera_type["get_y"]             = &graphics::Camera::get_y;
			camera_type["get_rotation"]      = &graphics::Camera::get_rotation;
			camera_type["get_view"]          = &graphics::Camera::get_view;
			camera_type["get_proj"]          = &graphics::Camera::get_proj;
			camera_type["get_zoom"]          = &graphics::Camera::get_zoom;
			camera_type["set_zoom"]          = &graphics::Camera::set_zoom;
			camera_type["translate"]         = &graphics::Camera::translate;
			camera_type["allow_rotate"]      = &graphics::Camera::m_allow_rotate;
			camera_type["rotation_speed"]    = &graphics::Camera::m_rotation_speed;
			camera_type["translation_speed"] = &graphics::Camera::m_translation_speed;

			auto frame_type              = lua.new_usertype<graphics::Frame>("Frame", sol::constructors<graphics::Frame()>());
			frame_type["texture_id"]     = &graphics::Frame::m_texture_id;
			frame_type["time_per_frame"] = &graphics::Frame::m_time_per_frame;

			auto animation_type                   = lua.new_usertype<graphics::Animation>("Animation",
                sol::constructors<graphics::Animation(), graphics::Animation(std::string_view, const bool, const double, std::span<graphics::Frame>)>());
			animation_type["current_frame"]       = &graphics::Animation::current_frame;
			animation_type["current_frame_index"] = &graphics::Animation::current_frame_index;
			animation_type["frames"]              = &graphics::Animation::m_frames;
			animation_type["looping"]             = &graphics::Animation::m_looping;
			animation_type["name"]                = &graphics::Animation::m_name;
			animation_type["speed"]               = &graphics::Animation::m_speed;
			animation_type["total_frames"]        = &graphics::Animation::m_total_frames;
			animation_type["next_frame"]          = &graphics::Animation::next_frame;
			animation_type["restart"]             = &graphics::Animation::restart;

			auto font_type                = lua.new_usertype<graphics::Font>("Font", sol::constructors<graphics::Font(), graphics::Font(const std::string&)>());
			font_type["build"]            = &graphics::Font::build;
			font_type["get_text_size"]    = &graphics::Font::get_text_size;
			font_type["load"]             = sol::resolve<bool(const std::string&)>(&graphics::Font::load);
			font_type["load_mem"]         = sol::resolve<bool(unsigned char*, const unsigned int)>(&graphics::Font::load);
			font_type["vertical_advance"] = &graphics::Font::vertical_advance;

			auto particle_type      = lua.new_usertype<graphics::Particle>("Particle", sol::constructors<graphics::Particle()>());
			particle_type["colour"] = &graphics::Particle::m_colour;
			particle_type["life"]   = &graphics::Particle::m_life;
			particle_type["pos"]    = &graphics::Particle::m_pos;
			particle_type["scale"]  = &graphics::Particle::m_scale;
			particle_type["vel"]    = &graphics::Particle::m_vel;

			/* INPUT */
			// clang-format off
			lua.new_enum<input::InputMods>("InputMods",
			{
				{"SHIFT", input::InputMods::SHIFT},
				{"CONTROL", input::InputMods::CONTROL},
				{"ALT", input::InputMods::ALT},
				{"SUPER", input::InputMods::SUPER},
				{"CAPS_LOCK", input::InputMods::CAPS_LOCK},
				{"NUM_LOCK", input::InputMods::NUM_LOCK}
			});
			// clang-format on

			// clang-format off
			lua.new_enum<input::Keys>("Keys",
			{
				{"UNKNOWN", input::Keys::UNKNOWN},
				{"SPACE", input::Keys::SPACE},
				{"APOSTROPHE", input::Keys::APOSTROPHE},
				{"COMMA", input::Keys::COMMA},
				{"MINUS", input::Keys::MINUS},
				{"PERIOD", input::Keys::PERIOD},
				{"SLASH", input::Keys::SLASH},
				{"_0", input::Keys::_0},
				{"_1", input::Keys::_1},
				{"_2", input::Keys::_2},
				{"_3", input::Keys::_3},
				{"_4", input::Keys::_4},
				{"_5", input::Keys::_5},
				{"_6", input::Keys::_6},
				{"_7", input::Keys::_7},
				{"_8", input::Keys::_8},
				{"_9", input::Keys::_9},
				{"SEMICOLON", input::Keys::SEMICOLON},
				{"EQUAL", input::Keys::EQUAL},
				{"A", input::Keys::A},
				{"B", input::Keys::B},
				{"C", input::Keys::C},
				{"D", input::Keys::D},
				{"E", input::Keys::E},
				{"F", input::Keys::F},
				{"G", input::Keys::G},
				{"H", input::Keys::H},
				{"I", input::Keys::I},
				{"J", input::Keys::J},
				{"K", input::Keys::K},
				{"L", input::Keys::L},
				{"M", input::Keys::M},
				{"N", input::Keys::N},
				{"O", input::Keys::O},
				{"P", input::Keys::P},
				{"Q", input::Keys::Q},
				{"R", input::Keys::R},
				{"S", input::Keys::S},
				{"T", input::Keys::T},
				{"U", input::Keys::U},
				{"V", input::Keys::V},
				{"W", input::Keys::W},
				{"X", input::Keys::X},
				{"Y", input::Keys::Y},
				{"Z", input::Keys::Z},
				{"LEFT_BRACKET", input::Keys::LEFT_BRACKET},
				{"BACKSLASH", input::Keys::BACKSLASH},
				{"RIGHT_BRACKET", input::Keys::RIGHT_BRACKET},
				{"GRAVE_ACCENT", input::Keys::GRAVE_ACCENT},
				{"WORLD_1", input::Keys::WORLD_1},
				{"WORLD_2", input::Keys::WORLD_2},
				{"ESCAPE", input::Keys::ESCAPE},
				{"ENTER", input::Keys::ENTER},
				{"TAB", input::Keys::TAB},
				{"BACKSPACE", input::Keys::BACKSPACE},
				{"INSERT", input::Keys::INSERT},
				{"DELETE", input::Keys::DELETE},
				{"RIGHT", input::Keys::RIGHT},
				{"LEFT", input::Keys::LEFT},
				{"DOWN", input::Keys::DOWN},
				{"UP", input::Keys::UP},
				{"PAGE_UP", input::Keys::PAGE_UP},
				{"PAGE_DOWN", input::Keys::PAGE_DOWN},
				{"HOME", input::Keys::HOME},
				{"END", input::Keys::END},
				{"CAPS_LOCK", input::Keys::CAPS_LOCK},
				{"SCROLL_LOCK", input::Keys::SCROLL_LOCK},
				{"NUM_LOCK", input::Keys::NUM_LOCK},
				{"PRINT_SCREEN", input::Keys::PRINT_SCREEN},
				{"PAUSE", input::Keys::PAUSE},
				{"F1", input::Keys::F1},
				{"F2", input::Keys::F2},
				{"F3", input::Keys::F3},
				{"F4", input::Keys::F4},
				{"F5", input::Keys::F5},
				{"F6", input::Keys::F6},
				{"F7", input::Keys::F7},
				{"F8", input::Keys::F8},
				{"F9", input::Keys::F9},
				{"F10", input::Keys::F10},
				{"F11", input::Keys::F11},
				{"F12", input::Keys::F12},
				{"F13", input::Keys::F13},
				{"F14", input::Keys::F14},
				{"F15", input::Keys::F15},
				{"F16", input::Keys::F16},
				{"F17", input::Keys::F17},
				{"F18", input::Keys::F18},
				{"F19", input::Keys::F19},
				{"F20", input::Keys::F20},
				{"F21", input::Keys::F21},
				{"F22", input::Keys::F22},
				{"F23", input::Keys::F23},
				{"F24", input::Keys::F24},
				{"F25", input::Keys::F25},
				{"KP_0", input::Keys::KP_0},
				{"KP_1", input::Keys::KP_1},
				{"KP_2", input::Keys::KP_2},
				{"KP_3", input::Keys::KP_3},
				{"KP_4", input::Keys::KP_4},
				{"KP_5", input::Keys::KP_5},
				{"KP_6", input::Keys::KP_6},
				{"KP_7", input::Keys::KP_7},
				{"KP_8", input::Keys::KP_8},
				{"KP_9", input::Keys::KP_9},
				{"KP_DECIMAL", input::Keys::KP_DECIMAL},
				{"KP_DIVIDE", input::Keys::KP_DIVIDE},
				{"KP_MULTIPLY", input::Keys::KP_MULTIPLY},
				{"KP_SUBTRACT", input::Keys::KP_SUBTRACT},
				{"KP_ADD", input::Keys::KP_ADD},
				{"KP_ENTER", input::Keys::KP_ENTER},
				{"KP_EQUAL", input::Keys::KP_EQUAL},
				{"LEFT_SHIFT", input::Keys::LEFT_SHIFT},
				{"LEFT_CONTROL", input::Keys::LEFT_CONTROL},
				{"LEFT_ALT", input::Keys::LEFT_ALT},
				{"LEFT_SUPER", input::Keys::LEFT_SUPER},
				{"RIGHT_SHIFT", input::Keys::RIGHT_SHIFT},
				{"RIGHT_CONTROL", input::Keys::RIGHT_CONTROL},
				{"RIGHT_ALT", input::Keys::RIGHT_ALT},
				{"RIGHT_SUPER", input::Keys::RIGHT_SUPER},
				{"MENU", input::Keys::MENU}
			});
			// clang-format on

			// clang-format off
			lua.new_enum<input::MouseButtons>("MouseButtons",
			{
                {"UNKNOWN", input::MouseButtons::UNKNOWN},
				{"BTN_1", input::MouseButtons::BTN_1},
				{"BTN_2", input::MouseButtons::BTN_2},
				{"BTN_3", input::MouseButtons::BTN_3},
				{"BTN_4", input::MouseButtons::BTN_4},
				{"BTN_5", input::MouseButtons::BTN_5},
				{"BTN_6", input::MouseButtons::BTN_6},
				{"BTN_7", input::MouseButtons::BTN_7},
				{"BTN_8", input::MouseButtons::BTN_8},
				{"BTN_LAST", input::MouseButtons::BTN_LAST},
				{"BTN_LEFT", input::MouseButtons::BTN_LEFT},
				{"BTN_RIGHT", input::MouseButtons::BTN_RIGHT},
				{"BTN_MIDDLE", input::MouseButtons::BTN_MIDDLE}
			});
			// clang-format on

			auto clipboard_type   = lua.new_usertype<input::Clipboard>("Clipboard", sol::no_constructor);
			clipboard_type["set"] = &input::Clipboard::set;
			clipboard_type["get"] = &input::Clipboard::get;

			auto cursor_type                       = lua.new_usertype<input::Cursor>("Cursor", sol::no_constructor);
			cursor_type["load_custom"]             = &input::Cursor::load_custom;
			cursor_type["toggle"]                  = &input::Cursor::toggle;
			cursor_type["within_window"]           = &input::Cursor::within_window;
			cursor_type["destroy"]                 = &input::Cursor::destroy;
			cursor_type["use_cross"]               = &input::Cursor::use_cross;
			cursor_type["use_custom"]              = &input::Cursor::use_custom;
			cursor_type["use_custom_else_pointer"] = &input::Cursor::use_custom_else_pointer;
			cursor_type["use_hand"]                = &input::Cursor::use_hand;
			cursor_type["use_pointer"]             = &input::Cursor::use_pointer;
			cursor_type["use_text"]                = &input::Cursor::use_text;

			lua.set_function("galaxy_key_down", &input::Input::key_down);
			lua.set_function("galaxy_mouse_down", &input::Input::mouse_button_down);
			lua.set_function("galaxy_cursor_pos", &input::Input::get_cursor_pos);

			auto keyboard_type                     = lua.new_usertype<input::Keyboard>("Keyboard", sol::no_constructor);
			keyboard_type["begin_text_input"]      = &input::Keyboard::begin_text_input;
			keyboard_type["disable_sticky_keys"]   = &input::Keyboard::disable_sticky_keys;
			keyboard_type["enable_sticky_keys"]    = &input::Keyboard::enable_sticky_keys;
			keyboard_type["end_text_input"]        = &input::Keyboard::end_text_input;
			keyboard_type["get_key_name"]          = &input::Keyboard::get_key_name;
			keyboard_type["get_scancode"]          = &input::Keyboard::get_scancode;
			keyboard_type["get_scancode_name"]     = &input::Keyboard::get_scancode_name;
			keyboard_type["is_text_input_enabled"] = &input::Keyboard::is_text_input_enabled;

			auto mouse_type                    = lua.new_usertype<input::Mouse>("Mouse", sol::no_constructor);
			mouse_type["disable_sticky_mouse"] = &input::Mouse::disable_sticky_mouse;
			mouse_type["enable_sticky_mouse"]  = &input::Mouse::enable_sticky_mouse;
			mouse_type["get_pos"]              = &input::Mouse::get_pos;

			auto& window            = core::ServiceLocator<core::Window>::ref();
			lua["galaxy_clipboard"] = std::ref(window.get_input<input::Clipboard>());
			lua["galaxy_cursor"]    = std::ref(window.get_input<input::Cursor>());
			lua["galaxy_keyboard"]  = std::ref(window.get_input<input::Keyboard>());
			lua["galaxy_mouse"]     = std::ref(window.get_input<input::Mouse>());

			/* MAP */
			auto tileanim_type   = lua.new_usertype<map::TileAnimation>("TileAnimation");
			auto map_type        = lua.new_usertype<map::Map>("Map", sol::constructors<map::Map(), map::Map(const std::string&)>());
			map_type["create"]   = &map::Map::create;
			map_type["disable"]  = &map::Map::disable;
			map_type["enable"]   = &map::Map::enable;
			map_type["get_name"] = &map::Map::get_name;
			map_type["load"]     = &map::Map::load;

			/* MATH */
			lua.set_function("normalize", &math::normalize<float>);
			lua.set_function("encode_base64", &math::encode_base64);
			lua.set_function("decode_base64", &math::decode_base64);
			lua.set_function("random_int", &math::random<int>);
			lua.set_function("random_float", &math::random<float>);
			lua.set_function("encode_zlib", &math::encode_zlib);
			lua.set_function("decode_zlib", &math::decode_zlib);
			lua.set_function("fnv1a_32", &math::fnv1a_32);
			lua.set_function("fnv1a_64", &math::fnv1a_64);

			// clang-format off
			lua.new_enum<math::ZLib::Mode>("ZLibMode",
			{
				{"COMPRESS", math::ZLib::Mode::COMPRESS},
				{"DECOMPRESS", math::ZLib::Mode::DECOMPRESS}
			});
			// clang-format on

			auto zlibclass_type          = lua.new_usertype<math::ZLib>("ZLib", sol::constructors<math::ZLib(math::ZLib::Mode)>());
			zlibclass_type["compress"]   = &math::ZLib::compress;
			zlibclass_type["decompress"] = &math::ZLib::decompress;
			zlibclass_type["finish"]     = &math::ZLib::finish;

			auto irect_type =
				lua.new_usertype<math::iRect>("iRect", sol::constructors<math::iRect(), math::iRect(const int, const int, const int, const int)>());
			irect_type["x"]        = &math::iRect::x;
			irect_type["y"]        = &math::iRect::y;
			irect_type["width"]    = &math::iRect::width;
			irect_type["height"]   = &math::iRect::height;
			irect_type["overlaps"] = &math::iRect::overlaps;
			// irect_type["contains_point"] = sol::resolve<bool(const int, const int)>(&math::iRect::contains);
			// irect_type["contains"]       = sol::resolve<bool(const math::iRect&)>(&math::iRect::contains);
			irect_type["get_bottom"]   = &math::iRect::get_bottom;
			irect_type["get_center"]   = &math::iRect::get_center;
			irect_type["get_right"]    = &math::iRect::get_right;
			irect_type["get_size"]     = &math::iRect::get_size;
			irect_type["get_top_left"] = &math::iRect::get_top_left;
			irect_type["intersects"]   = &math::iRect::intersects;

			auto frect_type =
				lua.new_usertype<math::fRect>("fRect", sol::constructors<math::fRect(), math::fRect(const float, const float, const float, const float)>());
			frect_type["x"]        = &math::fRect::x;
			frect_type["y"]        = &math::fRect::y;
			frect_type["width"]    = &math::fRect::width;
			frect_type["height"]   = &math::fRect::height;
			frect_type["overlaps"] = &math::fRect::overlaps;
			// irect_type["contains_point"] = sol::resolve<bool(const float, const float)>(&math::fRect::contains);
			// irect_type["contains"]       = sol::resolve<bool(const math::fRect&)>(&math::fRect::contains);
			frect_type["get_bottom"]   = &math::fRect::get_bottom;
			frect_type["get_center"]   = &math::fRect::get_center;
			frect_type["get_right"]    = &math::fRect::get_right;
			frect_type["get_size"]     = &math::fRect::get_size;
			frect_type["get_top_left"] = &math::fRect::get_top_left;
			frect_type["intersects"]   = &math::fRect::intersects;

			/* MEDIA */
			// clang-format off
			lua.new_enum<media::SoundType>("SoundType",
			{
				{"VOICE", media::SoundType::VOICE},
				{"MUSIC", media::SoundType::MUSIC},
				{"SFX", media::SoundType::SFX}
			});
			// clang-format on

			auto soundengine_type                      = lua.new_usertype<media::SoundEngine>("SoundEngine", sol::no_constructor);
			soundengine_type["set_listener_cone"]      = &media::SoundEngine::set_listener_cone;
			soundengine_type["set_listener_direction"] = &media::SoundEngine::set_listener_direction;
			soundengine_type["set_listener_position"]  = &media::SoundEngine::set_listener_position;
			soundengine_type["set_listener_world_up"]  = &media::SoundEngine::set_listener_world_up;
			soundengine_type["toggle_listener"]        = &media::SoundEngine::toggle_listener;
			soundengine_type["stop"]                   = &media::SoundEngine::stop;
			soundengine_type["find_closest_listener"]  = &media::SoundEngine::find_closest_listener;
			soundengine_type["get_channels"]           = &media::SoundEngine::get_channels;
			soundengine_type["get_listener_count"]     = &media::SoundEngine::get_listener_count;
			soundengine_type["get_samplerate"]         = &media::SoundEngine::get_samplerate;
			soundengine_type["is_listener_enabled"]    = &media::SoundEngine::is_listener_enabled;
			soundengine_type["set_listener_velocity"]  = &media::SoundEngine::set_listener_velocity;
			soundengine_type["set_volume"]             = &media::SoundEngine::set_volume;
			soundengine_type["start_device"]           = &media::SoundEngine::start_device;
			soundengine_type["stop_device"]            = &media::SoundEngine::stop_device;

			auto musicengine_type                      = lua.new_usertype<media::MusicEngine>("MusicEngine", sol::no_constructor);
			musicengine_type["set_listener_cone"]      = &media::MusicEngine::set_listener_cone;
			musicengine_type["set_listener_direction"] = &media::MusicEngine::set_listener_direction;
			musicengine_type["set_listener_position"]  = &media::MusicEngine::set_listener_position;
			musicengine_type["set_listener_world_up"]  = &media::MusicEngine::set_listener_world_up;
			musicengine_type["toggle_listener"]        = &media::MusicEngine::toggle_listener;
			musicengine_type["stop"]                   = &media::MusicEngine::stop;
			musicengine_type["find_closest_listener"]  = &media::MusicEngine::find_closest_listener;
			musicengine_type["get_channels"]           = &media::MusicEngine::get_channels;
			musicengine_type["get_listener_count"]     = &media::MusicEngine::get_listener_count;
			musicengine_type["get_samplerate"]         = &media::MusicEngine::get_samplerate;
			musicengine_type["is_listener_enabled"]    = &media::MusicEngine::is_listener_enabled;
			musicengine_type["set_listener_velocity"]  = &media::MusicEngine::set_listener_velocity;
			musicengine_type["set_volume"]             = &media::MusicEngine::set_volume;
			musicengine_type["start_device"]           = &media::MusicEngine::start_device;
			musicengine_type["stop_device"]            = &media::MusicEngine::stop_device;

			auto voiceengine_type                      = lua.new_usertype<media::VoiceEngine>("VoiceEngine", sol::no_constructor);
			voiceengine_type["set_listener_cone"]      = &media::VoiceEngine::set_listener_cone;
			voiceengine_type["set_listener_direction"] = &media::VoiceEngine::set_listener_direction;
			voiceengine_type["set_listener_position"]  = &media::VoiceEngine::set_listener_position;
			voiceengine_type["set_listener_world_up"]  = &media::VoiceEngine::set_listener_world_up;
			voiceengine_type["toggle_listener"]        = &media::VoiceEngine::toggle_listener;
			voiceengine_type["stop"]                   = &media::VoiceEngine::stop;
			voiceengine_type["find_closest_listener"]  = &media::VoiceEngine::find_closest_listener;
			voiceengine_type["get_channels"]           = &media::VoiceEngine::get_channels;
			voiceengine_type["get_listener_count"]     = &media::VoiceEngine::get_listener_count;
			voiceengine_type["get_samplerate"]         = &media::VoiceEngine::get_samplerate;
			voiceengine_type["is_listener_enabled"]    = &media::VoiceEngine::is_listener_enabled;
			voiceengine_type["set_listener_velocity"]  = &media::VoiceEngine::set_listener_velocity;
			voiceengine_type["set_volume"]             = &media::VoiceEngine::set_volume;
			voiceengine_type["start_device"]           = &media::VoiceEngine::start_device;
			voiceengine_type["stop_device"]            = &media::VoiceEngine::stop_device;

			auto sound_type                                  = lua.new_usertype<media::Sound>("Sound", sol::constructors<media::Sound()>());
			sound_type["fade_in"]                            = &media::Sound::fade_in;
			sound_type["fade_out"]                           = &media::Sound::fade_out;
			sound_type["is_finished"]                        = &media::Sound::is_finished;
			sound_type["is_looping"]                         = &media::Sound::is_looping;
			sound_type["is_playing"]                         = &media::Sound::is_playing;
			sound_type["load"]                               = &media::Sound::load;
			sound_type["set_attenuation_model"]              = &media::Sound::set_attenuation_model;
			sound_type["set_cone"]                           = &media::Sound::set_cone;
			sound_type["set_direction"]                      = &media::Sound::set_direction;
			sound_type["set_doppler_factor"]                 = &media::Sound::set_doppler_factor;
			sound_type["set_looping"]                        = &media::Sound::set_looping;
			sound_type["set_max_distance"]                   = &media::Sound::set_max_distance;
			sound_type["set_max_gain"]                       = &media::Sound::set_max_gain;
			sound_type["set_min_distance"]                   = &media::Sound::set_min_distance;
			sound_type["set_min_gain"]                       = &media::Sound::set_min_gain;
			sound_type["set_pan"]                            = &media::Sound::set_pan;
			sound_type["set_pinned_listener"]                = &media::Sound::set_pinned_listener;
			sound_type["set_pitch"]                          = &media::Sound::set_pitch;
			sound_type["set_position"]                       = &media::Sound::set_position;
			sound_type["set_positioning"]                    = &media::Sound::set_positioning;
			sound_type["set_rolloff"]                        = &media::Sound::set_rolloff;
			sound_type["set_velocity"]                       = &media::Sound::set_velocity;
			sound_type["set_volume"]                         = &media::Sound::set_volume;
			sound_type["play"]                               = &media::Sound::play;
			sound_type["pause"]                              = &media::Sound::pause;
			sound_type["stop"]                               = &media::Sound::stop;
			sound_type["get_attenuation_model"]              = &media::Sound::get_attenuation_model;
			sound_type["get_cone"]                           = &media::Sound::get_cone;
			sound_type["get_current_fade_volume"]            = &media::Sound::get_current_fade_volume;
			sound_type["get_cursor_in_seconds"]              = &media::Sound::get_cursor_in_seconds;
			sound_type["get_direction"]                      = &media::Sound::get_direction;
			sound_type["get_directional_attenuation_factor"] = &media::Sound::get_directional_attenuation_factor;
			sound_type["get_direction_to_listener"]          = &media::Sound::get_direction_to_listener;
			sound_type["get_doppler_factor"]                 = &media::Sound::get_doppler_factor;
			sound_type["get_length_in_seconds"]              = &media::Sound::get_length_in_seconds;
			sound_type["get_listener_index"]                 = &media::Sound::get_listener_index;
			sound_type["get_max_distance"]                   = &media::Sound::get_max_distance;
			sound_type["get_max_gain"]                       = &media::Sound::get_max_gain;
			sound_type["get_min_distance"]                   = &media::Sound::get_min_distance;
			sound_type["get_pan"]                            = &media::Sound::get_pan;
			sound_type["get_pan_mode"]                       = &media::Sound::get_pan_mode;
			sound_type["get_pinned_listener_index"]          = &media::Sound::get_pinned_listener_index;
			sound_type["get_pitch"]                          = &media::Sound::get_pitch;
			sound_type["get_position"]                       = &media::Sound::get_position;
			sound_type["get_positioning"]                    = &media::Sound::get_positioning;
			sound_type["get_rolloff"]                        = &media::Sound::get_rolloff;
			sound_type["get_velocity"]                       = &media::Sound::get_velocity;
			sound_type["get_volume"]                         = &media::Sound::get_volume;
			sound_type["is_spatialization_enabled"]          = &media::Sound::is_spatialization_enabled;
			sound_type["set_directional_attenuation_factor"] = &media::Sound::set_directional_attenuation_factor;
			sound_type["set_pan_mode"]                       = &media::Sound::set_pan_mode;
			sound_type["set_spatialization_enabled"]         = &media::Sound::set_spatialization_enabled;
			sound_type["set_start_time_in_milliseconds"]     = &media::Sound::set_start_time_in_milliseconds;
			sound_type["set_stop_time_in_milliseconds"]      = &media::Sound::set_stop_time_in_milliseconds;

			auto video_type              = lua.new_usertype<media::Video>("Video", sol::constructors<media::Video()>());
			video_type["get_duration"]   = &media::Video::get_duration;
			video_type["get_framerate"]  = &media::Video::get_framerate;
			video_type["get_samplerate"] = &media::Video::get_samplerate;
			video_type["is_finished"]    = &media::Video::is_finished;
			video_type["load"]           = &media::Video::load;
			video_type["render"]         = &media::Video::render;
			video_type["update"]         = &media::Video::update;
			video_type["get_time"]       = &media::Video::get_time;
			video_type["build"]          = &media::Video::build;

			/* META */
			auto entt_anytype     = lua.new_usertype<entt::any>("EnttAny", sol::no_constructor);
			entt_anytype["owner"] = &entt::any::owner;
			entt_anytype["reset"] = &entt::any::reset;
			entt_anytype["type"]  = &entt::any::type;

			auto entitymeta_type                   = lua.new_usertype<meta::EntityMeta>("EntityMeta", sol::no_constructor);
			entitymeta_type["any_from_json"]       = &meta::EntityMeta::any_from_json;
			entitymeta_type["get_type"]            = &meta::EntityMeta::get_type;
			entitymeta_type["get_typeid"]          = &meta::EntityMeta::get_typeid;
			entitymeta_type["get_validations"]     = &meta::EntityMeta::get_validations;
			entitymeta_type["get_validation_list"] = &meta::EntityMeta::get_validation_list;
			entitymeta_type["json_factory"]        = &meta::EntityMeta::json_factory;
			entitymeta_type["serialize_entity"]    = &meta::EntityMeta::serialize_entity;
			entitymeta_type["deserialize_entity"]  = &meta::EntityMeta::deserialize_entity;
			entitymeta_type["get_type"]            = &meta::EntityMeta::get_type;

			/* PLATFORM */
			auto subprocess_type         = lua.new_usertype<platform::Subprocess>("Subprocess",
                sol::constructors<platform::Subprocess(), platform::Subprocess(std::string_view, std::span<std::string> args)>());
			subprocess_type["alive"]     = &platform::Subprocess::alive;
			subprocess_type["create"]    = &platform::Subprocess::create;
			subprocess_type["destroy"]   = &platform::Subprocess::destroy;
			subprocess_type["join"]      = &platform::Subprocess::join;
			subprocess_type["terminate"] = &platform::Subprocess::terminate;

			/* RESOURCE */
			auto basicscripts_type     = lua.new_usertype<resource::BasicScripts>("BasicScripts", sol::no_constructor);
			basicscripts_type["clear"] = &resource::BasicScripts::clear;
			basicscripts_type["empty"] = &resource::BasicScripts::empty;
			basicscripts_type["get"]   = &resource::BasicScripts::get;
			basicscripts_type["has"]   = &resource::BasicScripts::has;
			basicscripts_type["keys"]  = &resource::BasicScripts::keys;
			basicscripts_type["size"]  = &resource::BasicScripts::size;

			auto fonts_type     = lua.new_usertype<resource::Fonts>("Fonts", sol::no_constructor);
			fonts_type["clear"] = &resource::Fonts::clear;
			fonts_type["empty"] = &resource::Fonts::empty;
			fonts_type["get"]   = &resource::Fonts::get;
			fonts_type["has"]   = &resource::Fonts::has;
			fonts_type["keys"]  = &resource::Fonts::keys;
			fonts_type["size"]  = &resource::Fonts::size;

			auto lang_type              = lua.new_usertype<resource::Language>("Language", sol::no_constructor);
			lang_type["clear_specific"] = sol::resolve<void(const std::string&)>(&resource::Language::clear);
			lang_type["clear"]          = sol::resolve<void(void)>(&resource::Language::clear);
			lang_type["set"]            = &resource::Language::set;
			lang_type["translate"]      = &resource::Language::translate;

			auto maps_type     = lua.new_usertype<resource::Maps>("Maps", sol::no_constructor);
			maps_type["clear"] = &resource::Maps::clear;
			maps_type["empty"] = &resource::Maps::empty;
			maps_type["get"]   = &resource::Maps::get;
			maps_type["has"]   = &resource::Maps::has;
			maps_type["keys"]  = &resource::Maps::keys;
			maps_type["size"]  = &resource::Maps::size;

			auto sfxcache_type     = lua.new_usertype<resource::SFXCache>("sfxcaches", sol::no_constructor);
			sfxcache_type["clear"] = &resource::SFXCache::clear;
			sfxcache_type["empty"] = &resource::SFXCache::empty;
			sfxcache_type["get"]   = &resource::SFXCache::get;
			sfxcache_type["has"]   = &resource::SFXCache::has;
			sfxcache_type["keys"]  = &resource::SFXCache::keys;
			sfxcache_type["size"]  = &resource::SFXCache::size;

			auto musiccache_type     = lua.new_usertype<resource::MusicCache>("MusicCache", sol::no_constructor);
			musiccache_type["clear"] = &resource::MusicCache::clear;
			musiccache_type["empty"] = &resource::MusicCache::empty;
			musiccache_type["get"]   = &resource::MusicCache::get;
			musiccache_type["has"]   = &resource::MusicCache::has;
			musiccache_type["keys"]  = &resource::MusicCache::keys;
			musiccache_type["size"]  = &resource::MusicCache::size;

			auto voicecache_type     = lua.new_usertype<resource::VoiceCache>("VoiceCache", sol::no_constructor);
			voicecache_type["clear"] = &resource::VoiceCache::clear;
			voicecache_type["empty"] = &resource::VoiceCache::empty;
			voicecache_type["get"]   = &resource::VoiceCache::get;
			voicecache_type["has"]   = &resource::VoiceCache::has;
			voicecache_type["keys"]  = &resource::VoiceCache::keys;
			voicecache_type["size"]  = &resource::VoiceCache::size;

			auto videocache_type     = lua.new_usertype<resource::VideoCache>("VideoCache", sol::no_constructor);
			videocache_type["clear"] = &resource::VideoCache::clear;
			videocache_type["empty"] = &resource::VideoCache::empty;
			videocache_type["get"]   = &resource::VideoCache::get;
			videocache_type["has"]   = &resource::VideoCache::has;
			videocache_type["keys"]  = &resource::VideoCache::keys;
			videocache_type["size"]  = &resource::VideoCache::size;

			auto prefabs_type     = lua.new_usertype<resource::Prefabs>("Prefabs", sol::no_constructor);
			prefabs_type["clear"] = &resource::Prefabs::clear;
			prefabs_type["empty"] = &resource::Prefabs::empty;
			prefabs_type["get"]   = &resource::Prefabs::get;
			prefabs_type["has"]   = &resource::Prefabs::has;
			prefabs_type["keys"]  = &resource::Prefabs::keys;
			prefabs_type["size"]  = &resource::Prefabs::size;

			auto shaders_type     = lua.new_usertype<resource::Shaders>("Shaders", sol::no_constructor);
			shaders_type["clear"] = &resource::Shaders::clear;
			shaders_type["empty"] = &resource::Shaders::empty;
			shaders_type["get"]   = &resource::Shaders::get;
			shaders_type["has"]   = &resource::Shaders::has;
			shaders_type["keys"]  = &resource::Shaders::keys;
			shaders_type["size"]  = &resource::Shaders::size;

			auto textureatlas_type        = lua.new_usertype<resource::TextureAtlas>("TextureAtlas", sol::no_constructor);
			textureatlas_type["clear"]    = &resource::TextureAtlas::clear;
			textureatlas_type["contains"] = &resource::TextureAtlas::contains;
			textureatlas_type["query"]    = &resource::TextureAtlas::query;
			textureatlas_type["save"]     = &resource::TextureAtlas::save;
			textureatlas_type["keys"]     = &resource::TextureAtlas::keys;

			/* STATE */
			auto scenemanager_type            = lua.new_usertype<scene::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["all"]          = &scene::SceneManager::all;
			scenemanager_type["clear"]        = &scene::SceneManager::clear;
			scenemanager_type["current"]      = &scene::SceneManager::current;
			scenemanager_type["get"]          = &scene::SceneManager::get;
			scenemanager_type["has_current"]  = &scene::SceneManager::has_current;
			scenemanager_type["load_appdata"] = &scene::SceneManager::load_appdata;
			scenemanager_type["load_scene"]   = &scene::SceneManager::load_scene;
			scenemanager_type["make_scene"]   = &scene::SceneManager::make_scene;
			scenemanager_type["remove"]       = &scene::SceneManager::remove;
			scenemanager_type["save_appdata"] = &scene::SceneManager::save_appdata;
			scenemanager_type["set_scene"]    = &scene::SceneManager::set_scene;
			scenemanager_type["unload_scene"] = &scene::SceneManager::unload_scene;

			// Use scenemanager to create.
			auto scene_type          = lua.new_usertype<scene::Scene>("Scene", sol::no_constructor);
			scene_type["camera"]     = &scene::Scene::m_camera;
			scene_type["dispatcher"] = &scene::Scene::m_dispatcher;
			scene_type["name"]       = &scene::Scene::m_name;
			scene_type["world"]      = &scene::Scene::m_world;

			/* SCRIPTING */
			auto basicscript_type              = lua.new_usertype<lua::BasicScript>("BasicScript", sol::constructors<lua::BasicScript()>());
			basicscript_type["load"]           = &lua::BasicScript::load;
			basicscript_type["run"]            = &lua::BasicScript::run;
			basicscript_type["run_and_return"] = &lua::BasicScript::run_and_return;

			/* STATE */
			auto state_type             = lua.new_usertype<state::State>("State", sol::no_constructor);
			auto statemachine_type      = lua.new_usertype<state::StateMachine>("StateMachine", sol::constructors<state::StateMachine()>());
			statemachine_type["pop"]    = &state::StateMachine::pop;
			statemachine_type["push"]   = &state::StateMachine::push;
			statemachine_type["update"] = &state::StateMachine::update;

			/* UI */
			auto nui_type             = lua.new_usertype<ui::NuklearUI>("NuklearUI", sol::no_constructor);
			nui_type["ctx"]           = &ui::NuklearUI::ctx;
			nui_type["disable_input"] = &ui::NuklearUI::disable_input;
			nui_type["enable_input"]  = &ui::NuklearUI::enable_input;
			nui_type["set_font"]      = &ui::NuklearUI::set_font;

			/* UTILS */
			auto guid_type         = lua.new_usertype<utils::Guid>("Guid", sol::constructors<utils::Guid()>());
			guid_type["as_string"] = &utils::Guid::to_string;
			guid_type["is_empty"]  = &utils::Guid::is_empty;

			lua.set("GALAXY_UPS", GALAXY_UPS);
			lua.set("GALAXY_DT", GALAXY_DT);
			lua.set("GALAXY_FLAG_BITSET_COUNT", GALAXY_FLAG_BITSET_COUNT);
			lua.set("GAlAXY_UBO_CAMERA_INDEX", GAlAXY_UBO_CAMERA_INDEX);
			lua.set("GAlAXY_UBO_R2D_INDEX", GAlAXY_UBO_R2D_INDEX);
			lua.set("GALAXY_SSBO_LIGHTING_INDEX", GALAXY_SSBO_LIGHTING_INDEX);
			lua.set("GALAXY_MIN_CAMERA_ZOOM", GALAXY_MIN_CAMERA_ZOOM);
			lua.set("GALAXY_MAX_CAMERA_ZOOM", GALAXY_MAX_CAMERA_ZOOM);
			lua.set("GALAXY_FONT_MSDF_RANGE", GALAXY_FONT_MSDF_RANGE);
			lua.set("GALAXY_FONT_MSDF_SCALE", GALAXY_FONT_MSDF_SCALE);
			lua.set("GALAXY_DEFAULT_ELLIPSE_FRAGMENTS", GALAXY_DEFAULT_ELLIPSE_FRAGMENTS);
			lua.set("GALAXY_ROOT_DIR", GALAXY_ROOT_DIR.string());
			lua.set("GALAXY_DATA_DIR", GALAXY_DATA_DIR);
			lua.set("GALAXY_WORK_DIR", GALAXY_WORK_DIR);
			lua.set("GALAXY_MOD_DIR", GALAXY_MOD_DIR);
			lua.set("GALAXY_UPDATE_DIR", GALAXY_UPDATE_DIR);
			lua.set("GALAXY_MUSIC_DIR", GALAXY_MUSIC_DIR);
			lua.set("GALAXY_SFX_DIR", GALAXY_SFX_DIR);
			lua.set("GALAXY_VOICE_DIR", GALAXY_VOICE_DIR);
			lua.set("GALAXY_FONT_DIR", GALAXY_FONT_DIR);
			lua.set("GALAXY_SCRIPT_DIR", GALAXY_SCRIPT_DIR);
			lua.set("GALAXY_SHADER_DIR", GALAXY_SHADER_DIR);
			lua.set("GALAXY_TEXTURE_DIR", GALAXY_TEXTURE_DIR);
			lua.set("GALAXY_ATLAS_DIR", GALAXY_ATLAS_DIR);
			lua.set("GALAXY_LANG_DIR", GALAXY_LANG_DIR);
			lua.set("GALAXY_PREFABS_DIR", GALAXY_PREFABS_DIR);
			lua.set("GALAXY_MAPS_DIR", GALAXY_MAPS_DIR);
			lua.set("GALAXY_VIDEO_DIR", GALAXY_VIDEO_DIR);
			lua.set("GALAXY_UI_DIR", GALAXY_UI_DIR);
			lua.set("GALAXY_UI_FONTS_DIR", GALAXY_UI_FONTS_DIR);
			lua.set("GALAXY_EDITOR_DATA_DIR", GALAXY_EDITOR_DATA_DIR);
			lua.set("GALAXY_ZLIB_COMPLETE_CHUNK", GALAXY_ZLIB_COMPLETE_CHUNK);
			lua.set("GALAXY_EXIT_SUCCESS", GALAXY_EXIT_SUCCESS);
			lua.set("GALAXY_EXIT_FAILURE", GALAXY_EXIT_FAILURE);
			lua.set("GALAXY_WORLD_TO_BOX", GALAXY_WORLD_TO_BOX);
			lua.set("GALAXY_BOX_TO_WORLD", GALAXY_BOX_TO_WORLD);

			lua.set_function("galaxy_str_begins_with", &strutils::begins_with);
		}
	} // namespace lua
} // namespace galaxy
