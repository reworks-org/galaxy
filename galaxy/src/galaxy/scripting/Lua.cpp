///
/// Lua.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entt.hpp>
#include <entt/addons/sol/dispatcher.hpp>
#include <entt/addons/sol/registry.hpp>

#include "galaxy/algorithm/Algorithms.hpp"
#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/GZip.hpp"
#include "galaxy/algorithm/Random.hpp"
#include "galaxy/algorithm/ZLib.hpp"

#include "galaxy/audio/AudioEngine.hpp"

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/core/World.hpp"

#include "galaxy/error/Log.hpp"

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "galaxy/fs/FileInfo.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "galaxy/flags/AllowSerialize.hpp"
#include "galaxy/flags/Enabled.hpp"

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/Shader.hpp"

#include "galaxy/input/Clipboard.hpp"
#include "galaxy/input/Cursor.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Keys.hpp"
#include "galaxy/input/Mouse.hpp"
#include "galaxy/input/MouseButtons.hpp"

#include "galaxy/platform/Subprocess.hpp"

#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "galaxy/state/SceneManager.hpp"

#include "galaxy/scripting/BasicScript.hpp"

#include "galaxy/utils/Globals.hpp"
#include "galaxy/utils/Guid.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Lua.hpp"

namespace galaxy
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

	namespace lua
	{
		void inject_galaxy_into_lua()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			/* ENTT */
			lua.require("registry", sol::c_call<decltype(&entt_sol::open_registry), &entt_sol::open_registry>, false);
			lua.require("dispatcher", sol::c_call<decltype(&entt_sol::open_dispatcher), &entt_sol::open_dispatcher>, false);

			/* GLM */
			auto vec2_type = lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2()>());
			vec2_type["x"] = &glm::vec2::x;
			vec2_type["y"] = &glm::vec2::y;

			auto dvec2_type = lua.new_usertype<glm::dvec2>("dvec2", sol::constructors<glm::dvec2()>());
			dvec2_type["x"] = &glm::dvec2::x;
			dvec2_type["y"] = &glm::dvec2::y;

			auto vec3_type = lua.new_usertype<glm::vec3>("vec3", sol::constructors<glm::vec3()>());
			vec3_type["x"] = &glm::vec3::x;
			vec3_type["y"] = &glm::vec3::y;
			vec3_type["z"] = &glm::vec3::z;

			auto vec4_type = lua.new_usertype<glm::vec4>("vec4", sol::constructors<glm::vec4()>());
			vec4_type["x"] = &glm::vec4::x;
			vec4_type["y"] = &glm::vec4::y;
			vec4_type["z"] = &glm::vec4::z;
			vec4_type["w"] = &glm::vec4::w;

			auto mat4_type      = lua.new_usertype<glm::mat4>("mat4", sol::constructors<glm::mat4()>());
			mat4_type["length"] = &glm::mat4::length;

			/* ALGORITHM */
			lua.set_function("normalize", &algorithm::normalize<float>);
			lua.set_function("encode_base64", &algorithm::encode_base64);
			lua.set_function("decode_base64", &algorithm::decode_base64);
			lua.set_function("encode_gzip", &algorithm::encode_gzip);
			lua.set_function("decode_gzip", &algorithm::decode_gzip);
			lua.set_function("random_int", &algorithm::random<int>);
			lua.set_function("random_float", &algorithm::random<float>);
			lua.set_function("encode_zlib", &algorithm::encode_zlib);
			lua.set_function("decode_zlib", &algorithm::decode_zlib);

			/* AUDIO */
			auto audio_type          = lua.new_usertype<audio::Audio>("Audio", sol::no_constructor);
			audio_type["play"]       = &audio::Audio::play;
			audio_type["stop"]       = &audio::Audio::stop;
			audio_type["is_playing"] = &audio::Audio::is_playing;

			auto audioengine_type                = lua.new_usertype<audio::AudioEngine>("AudioEngine", sol::no_constructor);
			audioengine_type["make_sfx"]         = &audio::AudioEngine::make_sfx;
			audioengine_type["make_music"]       = &audio::AudioEngine::make_music;
			audioengine_type["make_voice"]       = &audio::AudioEngine::make_voice;
			audioengine_type["set_sfx_volume"]   = &audio::AudioEngine::set_sfx_volume;
			audioengine_type["set_music_volume"] = &audio::AudioEngine::set_music_volume;
			audioengine_type["set_voice_volume"] = &audio::AudioEngine::set_voice_volume;
			audioengine_type["stop_all"]         = &audio::AudioEngine::stop_all;
			audioengine_type["toggle_pause_all"] = &audio::AudioEngine::toggle_pause_all;

			lua["galaxy_audioengine"] = std::ref(core::ServiceLocator<audio::AudioEngine>::ref());

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

			entt_sol::register_meta_component<components::Animated>();

			auto drawshader_type = lua.new_usertype<components::DrawShader>("DrawShader",
				sol::constructors<components::DrawShader()>(),
				"type_id",
				&entt::type_hash<components::DrawShader>::value);

			drawshader_type["shader"]     = &components::DrawShader::m_shader;
			drawshader_type["set_shader"] = &components::DrawShader::set_shader;
			drawshader_type["id"]         = &components::DrawShader::id;

			entt_sol::register_meta_component<components::DrawShader>();

			auto flag_type =
				lua.new_usertype<components::Flag>("Flag", sol::constructors<components::Flag()>(), "type_id", &entt::type_hash<components::Animated>::value);
			flag_type["set_enabled"]            = &components::Flag::set_flag<flags::Enabled>;
			flag_type["unset_enabled"]          = &components::Flag::unset_flag<flags::Enabled>;
			flag_type["is_enabled_set"]         = &components::Flag::is_flag_set<flags::Enabled>;
			flag_type["set_allow_serialize"]    = &components::Flag::set_flag<flags::AllowSerialize>;
			flag_type["unset_allow_serialize"]  = &components::Flag::unset_flag<flags::AllowSerialize>;
			flag_type["is_allow_serialize_set"] = &components::Flag::is_flag_set<flags::AllowSerialize>;

			entt_sol::register_meta_component<components::Flag>();

			auto mapcomponent_type =
				lua.new_usertype<components::Map>("Map", sol::constructors<components::Map()>(), "type_id", &entt::type_hash<components::Map>::value);
			// Note: Nothing needs to be registered with lua.

			entt_sol::register_meta_component<components::Map>();

			auto primitive_data_type =
				lua.new_usertype<components::Primitive::PrimitiveData>("PrimitiveData", sol::constructors<components::Primitive::PrimitiveData()>());
			primitive_data_type["fragments"] = &components::Primitive::PrimitiveData::m_fragments;
			primitive_data_type["points"]    = &components::Primitive::PrimitiveData::m_points;
			primitive_data_type["radii"]     = &components::Primitive::PrimitiveData::m_radii;
			primitive_data_type["radius"]    = &components::Primitive::PrimitiveData::m_radius;
			primitive_data_type["start_end"] = &components::Primitive::PrimitiveData::m_start_end;

			auto primitive_type = lua.new_usertype<components::Primitive>("Primitive",
				sol::constructors<components::Primitive()>(),
				"type_id",
				&entt::type_hash<components::Primitive>::value);

			primitive_type["create_circle"]   = &components::Primitive::create<graphics::Shape::CIRCLE>;
			primitive_type["create_ellipse"]  = &components::Primitive::create<graphics::Shape::ELLIPSE>;
			primitive_type["create_line"]     = &components::Primitive::create<graphics::Shape::LINE>;
			primitive_type["create_point"]    = &components::Primitive::create<graphics::Shape::POINT>;
			primitive_type["create_polygon"]  = &components::Primitive::create<graphics::Shape::POLYGON>;
			primitive_type["create_polyline"] = &components::Primitive::create<graphics::Shape::POLYLINE>;
			primitive_type["get_data"]        = &components::Primitive::get_data;
			primitive_type["get_height"]      = &components::Primitive::get_height;
			primitive_type["get_shape"]       = &components::Primitive::get_shape;
			primitive_type["get_width"]       = &components::Primitive::get_width;
			primitive_type["colour"]          = &components::Primitive::m_colour;

			entt_sol::register_meta_component<components::Primitive>();

			auto script_type = lua.new_usertype<components::Script>("Script",
				sol::constructors<components::Script()>(),
				"type_id",
				&entt::type_hash<components::Script>::value);

			script_type["file"]   = &components::Script::file;
			script_type["load"]   = &components::Script::load;
			script_type["self"]   = &components::Script::m_self;
			script_type["update"] = &components::Script::m_update;

			entt_sol::register_meta_component<components::Script>();

			auto sprite_type = lua.new_usertype<components::Sprite>("Sprite",
				sol::constructors<components::Sprite()>(),
				"type_id",
				&entt::type_hash<components::Sprite>::value);

			sprite_type["create"] = sol::resolve<void(const std::string&, const int, const float)>(&components::Sprite::create);
			sprite_type["create_texturerect"] =
				sol::resolve<void(const std::string&, const graphics::iRect&, const int, const float)>(&components::Sprite::create);
			sprite_type["update"]             = sol::resolve<void(const std::string&)>(&components::Sprite::update);
			sprite_type["update_texturerect"] = sol::resolve<void(const std::string&, const graphics::iRect&)>(&components::Sprite::update);
			sprite_type["get_height"]         = &components::Sprite::get_height;
			sprite_type["get_opacity"]        = &components::Sprite::get_opacity;
			sprite_type["get_width"]          = &components::Sprite::get_width;
			sprite_type["set_opacity"]        = &components::Sprite::set_opacity;

			entt_sol::register_meta_component<components::Sprite>();

			auto tag_type =
				lua.new_usertype<components::Tag>("Tag", sol::constructors<components::Tag()>(), "type_id", &entt::type_hash<components::Tag>::value);
			tag_type["tag"] = &components::Tag::m_tag;

			entt_sol::register_meta_component<components::Tag>();

			auto text_type =
				lua.new_usertype<components::Text>("Text", sol::constructors<components::Text()>(), "type_id", &entt::type_hash<components::Text>::value);
			text_type["create"]     = &components::Text::create;
			text_type["get_height"] = &components::Text::get_height;
			text_type["get_size"]   = &components::Text::get_size;
			text_type["get_text"]   = &components::Text::get_text;
			text_type["get_width"]  = &components::Text::get_width;
			text_type["get_width"]  = &components::Text::get_font;
			text_type["colour"]     = &components::Text::m_colour;

			text_type["update_text"]             = sol::resolve<void(std::string_view)>(&components::Text::update);
			text_type["update_text_size"]        = sol::resolve<void(std::string_view, const float)>(&components::Text::update);
			text_type["update_text_colour"]      = sol::resolve<void(std::string_view, const graphics::Colour&)>(&components::Text::update);
			text_type["update_text_size_colour"] = sol::resolve<void(std::string_view, const float, const graphics::Colour&)>(&components::Text::update);

			entt_sol::register_meta_component<components::Text>();

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

			entt_sol::register_meta_component<components::Transform>();

			/* CORE */
			auto prefab_type    = lua.new_usertype<core::Prefab>("Prefab", sol::constructors<core::Prefab()>());
			prefab_type["data"] = &core::Prefab::m_data;

			auto config_type                 = lua.new_usertype<core::Config>("Config", sol::no_constructor);
			config_type["load"]              = &core::Config::load;
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

			lua["service_config"] = std::ref(core::ServiceLocator<core::Config>::ref());

			// Cannot be created in lua, accessed from scene instead.
			auto world_type                  = lua.new_usertype<core::World>("World", sol::no_constructor);
			world_type["clear"]              = &core::World::clear;
			world_type["registry"]           = &core::World::m_registry;
			world_type["dispatcher"]         = &core::World::m_dispatcher;
			world_type["create"]             = &core::World::create;
			world_type["create_from_prefab"] = &core::World::create_from_prefab;
			world_type["is_valid"]           = &core::World::is_valid;

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
			auto keychar_type       = lua.new_usertype<events::KeyChar>("KeyChar", sol::constructors<events::KeyChar()>());
			keychar_type["type_id"] = &entt::type_hash<events::KeyChar>::value;
			keychar_type["char"]    = &events::KeyChar::m_char;
			keychar_type["uichar"]  = &events::KeyChar::m_uichar;
			keychar_type["handled"] = &events::KeyChar::m_handled;

			entt_sol::register_meta_event<events::KeyChar>();

			auto keydown_type        = lua.new_usertype<events::KeyDown>("KeyDown", sol::constructors<events::KeyDown()>());
			keydown_type["type_id"]  = &entt::type_hash<events::KeyDown>::value;
			keydown_type["keycode"]  = &events::KeyDown::m_keycode;
			keydown_type["mod"]      = &events::KeyDown::m_mod;
			keydown_type["scancode"] = &events::KeyDown::m_scancode;
			keydown_type["handled"]  = &events::KeyDown::m_handled;

			entt_sol::register_meta_event<events::KeyDown>();

			auto keyup_type        = lua.new_usertype<events::KeyUp>("KeyUp", sol::constructors<events::KeyUp()>());
			keyup_type["type_id"]  = &entt::type_hash<events::KeyUp>::value;
			keyup_type["keycode"]  = &events::KeyUp::m_keycode;
			keyup_type["mod"]      = &events::KeyUp::m_mod;
			keyup_type["scancode"] = &events::KeyUp::m_scancode;
			keyup_type["handled"]  = &events::KeyUp::m_handled;

			entt_sol::register_meta_event<events::KeyUp>();

			auto mousemoved_type       = lua.new_usertype<events::MouseMoved>("MouseMoved", sol::constructors<events::MouseMoved()>());
			mousemoved_type["type_id"] = &entt::type_hash<events::MouseMoved>::value;
			mousemoved_type["xpos"]    = &events::MouseMoved::m_xpos;
			mousemoved_type["ypos"]    = &events::MouseMoved::m_ypos;
			mousemoved_type["handled"] = &events::MouseMoved::m_handled;

			entt_sol::register_meta_event<events::MouseMoved>();

			auto mousepressed_type       = lua.new_usertype<events::MousePressed>("MousePressed", sol::constructors<events::MousePressed()>());
			mousepressed_type["type_id"] = &entt::type_hash<events::MousePressed>::value;
			mousepressed_type["button"]  = &events::MousePressed::m_button;
			mousepressed_type["mod"]     = &events::MousePressed::m_mod;
			mousepressed_type["xpos"]    = &events::MousePressed::m_xpos;
			mousepressed_type["ypos"]    = &events::MousePressed::m_ypos;
			mousepressed_type["handled"] = &events::MousePressed::m_handled;

			entt_sol::register_meta_event<events::MousePressed>();

			auto mousereleased_type       = lua.new_usertype<events::MouseReleased>("MouseReleased", sol::constructors<events::MouseReleased()>());
			mousereleased_type["type_id"] = &entt::type_hash<events::MouseReleased>::value;
			mousereleased_type["button"]  = &events::MouseReleased::m_button;
			mousereleased_type["mod"]     = &events::MouseReleased::m_mod;
			mousereleased_type["xpos"]    = &events::MouseReleased::m_xpos;
			mousereleased_type["ypos"]    = &events::MouseReleased::m_ypos;
			mousereleased_type["handled"] = &events::MouseReleased::m_handled;

			entt_sol::register_meta_event<events::MouseReleased>();

			auto mousewheel_type       = lua.new_usertype<events::MouseWheel>("MouseWheel", sol::constructors<events::MouseWheel()>());
			mousewheel_type["type_id"] = &entt::type_hash<events::MouseWheel>::value;
			mousewheel_type["xoff"]    = &events::MouseWheel::m_xoff;
			mousewheel_type["yoff"]    = &events::MouseWheel::m_yoff;
			mousewheel_type["handled"] = &events::MouseWheel::m_handled;

			entt_sol::register_meta_event<events::MouseWheel>();

			auto windowresized_type       = lua.new_usertype<events::WindowResized>("WindowResized", sol::constructors<events::WindowResized()>());
			windowresized_type["type_id"] = &entt::type_hash<events::WindowResized>::value;
			windowresized_type["width"]   = &events::WindowResized::m_width;
			windowresized_type["height"]  = &events::WindowResized::m_height;

			entt_sol::register_meta_event<events::WindowResized>();

			/* FS */
			// clang-format off
			lua.new_enum<fs::FileCode>("FileInfoCodes",
			{
				{"FOUND", fs::FileCode::FOUND},
				{"NOT_FOUND", fs::FileCode::NOT_FOUND},
				{"NOT_IN_VFS", fs::FileCode::NOT_IN_VFS},
				{"NO_EXTENSION", fs::FileCode::NO_EXTENSION}
			});
			// clang-format on

			auto fileinfo_type      = lua.new_usertype<fs::FileInfo>("FileInfo", sol::constructors<fs::FileInfo()>());
			fileinfo_type["code"]   = &fs::FileInfo::m_code;
			fileinfo_type["string"] = &fs::FileInfo::m_string;

			auto vfs_type             = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["find"]          = &fs::VirtualFileSystem::find;
			vfs_type["create_file"]   = &fs::VirtualFileSystem::create_file;
			vfs_type["create_folder"] = &fs::VirtualFileSystem::create_folder;
			vfs_type["open"]          = &fs::VirtualFileSystem::open;
			vfs_type["open_binary"]   = &fs::VirtualFileSystem::open_binary;
			vfs_type["save"]          = &fs::VirtualFileSystem::save;
			vfs_type["save_binary"]   = &fs::VirtualFileSystem::save_binary;
			vfs_type["remove"]        = &fs::VirtualFileSystem::remove;
			vfs_type["root"]          = &fs::VirtualFileSystem::root;

			lua["service_fs"] = std::ref(core::ServiceLocator<fs::VirtualFileSystem>::ref());

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
			lua.new_enum<graphics::Shape>("Shape",
			{
				{"CIRCLE", graphics::Shape::CIRCLE},
				{"ELLIPSE", graphics::Shape::ELLIPSE},
				{"LINE", graphics::Shape::LINE},
				{"POINT", graphics::Shape::POINT},
				{"POLYGON", graphics::Shape::POLYGON},
				{"POLYLINE", graphics::Shape::POLYLINE}
			});
			// clang-format on

			auto colour_type          = lua.new_usertype<graphics::Colour>("Colour",
                sol::constructors<graphics::Colour(), graphics::Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
			colour_type["alpha"]      = &graphics::Colour::m_alpha;
			colour_type["blue"]       = &graphics::Colour::m_blue;
			colour_type["green"]      = &graphics::Colour::m_green;
			colour_type["red"]        = &graphics::Colour::m_red;
			colour_type["normalized"] = &graphics::Colour::normalized;

			auto irect_type =
				lua.new_usertype<graphics::iRect>("iRect", sol::constructors<graphics::iRect(), graphics::iRect(const int, const int, const int, const int)>());
			irect_type["contains"] = sol::resolve<bool(const int, const int)>(&graphics::iRect::contains);
			irect_type["height"]   = &graphics::iRect::m_height;
			irect_type["width"]    = &graphics::iRect::m_width;
			irect_type["x"]        = &graphics::iRect::m_x;
			irect_type["y"]        = &graphics::iRect::m_y;
			irect_type["overlaps"] = &graphics::iRect::overlaps;

			auto frect_type        = lua.new_usertype<graphics::fRect>("fRect",
                sol::constructors<graphics::fRect(), graphics::fRect(const float, const float, const float, const float)>());
			frect_type["contains"] = sol::resolve<bool(const float, const float)>(&graphics::fRect::contains);
			frect_type["height"]   = &graphics::fRect::m_height;
			frect_type["width"]    = &graphics::fRect::m_width;
			frect_type["x"]        = &graphics::fRect::m_x;
			frect_type["y"]        = &graphics::fRect::m_y;
			frect_type["overlaps"] = &graphics::fRect::overlaps;

			auto uniform_type        = lua.new_usertype<graphics::UniformInfo>("UniformInfo", sol::constructors<graphics::UniformInfo>());
			uniform_type["count"]    = &graphics::UniformInfo::m_count;
			uniform_type["location"] = &graphics::UniformInfo::m_location;

			auto shader_type =
				lua.new_usertype<graphics::Shader>("Shader", sol::constructors<graphics::Shader(), graphics::Shader(std::string_view, std::string_view)>());
			shader_type["id"]                   = &graphics::Shader::id;
			shader_type["get_uniform_count"]    = &graphics::Shader::get_uniform_count;
			shader_type["get_uniform_info"]     = &graphics::Shader::get_uniform_info;
			shader_type["get_uniform_location"] = &graphics::Shader::get_uniform_location;
			shader_type["load_file"]            = &graphics::Shader::load_file;
			shader_type["load_raw"]             = &graphics::Shader::load_raw;
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

			auto camera_type                 = lua.new_usertype<graphics::Camera>("Camera", sol::constructors<graphics::Camera(bool)>());
			camera_type["set_projection"]    = &graphics::Camera::set_projection;
			camera_type["set_pos"]           = &graphics::Camera::set_pos;
			camera_type["set_rotation"]      = &graphics::Camera::set_rotation;
			camera_type["get_x"]             = &graphics::Camera::get_x;
			camera_type["get_y"]             = &graphics::Camera::get_y;
			camera_type["get_rotation"]      = &graphics::Camera::get_rotation;
			camera_type["get_view"]          = &graphics::Camera::get_view;
			camera_type["get_proj"]          = &graphics::Camera::get_proj;
			camera_type["get_zoom"]          = &graphics::Camera::get_zoom;
			camera_type["on_mouse_wheel"]    = &graphics::Camera::on_mouse_wheel;
			camera_type["on_window_resized"] = &graphics::Camera::on_window_resized;
			camera_type["process_events"]    = &graphics::Camera::process_events;
			camera_type["set_zoom"]          = &graphics::Camera::set_zoom;
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
			font_type["get_text_height"]  = &graphics::Font::get_text_height;
			font_type["get_text_size"]    = &graphics::Font::get_text_size;
			font_type["get_text_width"]   = &graphics::Font::get_text_width;
			font_type["load"]             = &graphics::Font::load;
			font_type["vertical_advance"] = &graphics::Font::vertical_advance;

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

			auto cursor_type               = lua.new_usertype<input::Cursor>("Cursor", sol::no_constructor);
			cursor_type["set_cursor_icon"] = sol::resolve<void(std::string_view)>(&input::Cursor::set_cursor_icon);
			cursor_type["toggle"]          = &input::Cursor::toggle;
			cursor_type["within_window"]   = &input::Cursor::within_window;

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

			/* PLATFORM */
			auto subprocess_type         = lua.new_usertype<platform::Subprocess>("Subprocess",
                sol::constructors<platform::Subprocess(), platform::Subprocess(std::string_view, std::span<std::string> args)>());
			subprocess_type["alive"]     = &platform::Subprocess::alive;
			subprocess_type["create"]    = &platform::Subprocess::create;
			subprocess_type["destroy"]   = &platform::Subprocess::destroy;
			subprocess_type["join"]      = &platform::Subprocess::join;
			subprocess_type["terminate"] = &platform::Subprocess::terminate;

			/* RESOURCE */
			auto lang_type         = lua.new_usertype<resource::Language>("Language", sol::no_constructor);
			lang_type["load"]      = &resource::Language::load;
			lang_type["reload"]    = &resource::Language::reload;
			lang_type["set"]       = &resource::Language::set;
			lang_type["translate"] = &resource::Language::translate;
			lang_type["clear"]     = &resource::Language::clear;

			auto maps_type     = lua.new_usertype<resource::Maps>("Maps", sol::no_constructor);
			maps_type["clear"] = &resource::Maps::clear;
			maps_type["empty"] = &resource::Maps::empty;
			maps_type["get"]   = &resource::Maps::get;
			maps_type["has"]   = &resource::Maps::has;
			maps_type["load"]  = &resource::Maps::load;
			maps_type["keys"]  = &resource::Maps::keys;

			auto prefabs_type     = lua.new_usertype<resource::Prefabs>("Prefabs", sol::no_constructor);
			prefabs_type["clear"] = &resource::Prefabs::clear;
			prefabs_type["empty"] = &resource::Prefabs::empty;
			prefabs_type["get"]   = &resource::Prefabs::get;
			prefabs_type["has"]   = &resource::Prefabs::has;
			prefabs_type["load"]  = &resource::Prefabs::load;
			prefabs_type["keys"]  = &resource::Prefabs::keys;

			auto scripts_type     = lua.new_usertype<resource::Scripts>("Scripts", sol::no_constructor);
			scripts_type["clear"] = &resource::Scripts::clear;
			scripts_type["empty"] = &resource::Scripts::empty;
			scripts_type["get"]   = &resource::Scripts::get;
			scripts_type["has"]   = &resource::Scripts::has;
			scripts_type["load"]  = &resource::Scripts::load;
			scripts_type["keys"]  = &resource::Scripts::keys;

			auto shaders_type       = lua.new_usertype<resource::Shaders>("Shaders", sol::no_constructor);
			shaders_type["clear"]   = &resource::Shaders::clear;
			shaders_type["empty"]   = &resource::Shaders::empty;
			shaders_type["get"]     = &resource::Shaders::get;
			shaders_type["has"]     = &resource::Shaders::has;
			shaders_type["load"]    = &resource::Shaders::load;
			shaders_type["compile"] = &resource::Shaders::compile;
			shaders_type["keys"]    = &resource::Shaders::keys;

			auto sounds_type             = lua.new_usertype<resource::Sounds>("Sounds", sol::no_constructor);
			sounds_type["clear"]         = &resource::Sounds::clear;
			sounds_type["empty"]         = &resource::Sounds::empty;
			sounds_type["get"]           = &resource::Sounds::get;
			sounds_type["has"]           = &resource::Sounds::has;
			sounds_type["load_music"]    = &resource::Sounds::load_music;
			sounds_type["load_sfx"]      = &resource::Sounds::load_sfx;
			sounds_type["load_dialogue"] = &resource::Sounds::load_dialogue;
			sounds_type["keys"]          = &resource::Sounds::keys;

			auto textureatlas_type          = lua.new_usertype<resource::TextureAtlas>("TextureAtlas", sol::no_constructor);
			textureatlas_type["add_file"]   = &resource::TextureAtlas::add_file;
			textureatlas_type["add_folder"] = &resource::TextureAtlas::add_folder;
			textureatlas_type["clear"]      = &resource::TextureAtlas::clear;
			textureatlas_type["contains"]   = &resource::TextureAtlas::contains;
			textureatlas_type["query"]      = &resource::TextureAtlas::query;
			textureatlas_type["reload"]     = &resource::TextureAtlas::reload;
			textureatlas_type["save"]       = &resource::TextureAtlas::save;
			textureatlas_type["keys"]       = &resource::TextureAtlas::keys;

			auto fonts_type     = lua.new_usertype<resource::Fonts>("Fonts", sol::no_constructor);
			fonts_type["clear"] = &resource::Fonts::clear;
			fonts_type["empty"] = &resource::Fonts::empty;
			fonts_type["get"]   = &resource::Fonts::get;
			fonts_type["has"]   = &resource::Fonts::has;
			fonts_type["load"]  = &resource::Fonts::load;
			fonts_type["build"] = &resource::Fonts::build;
			fonts_type["keys"]  = &resource::Fonts::keys;

			/* STATE */
			auto scenemanager_type       = lua.new_usertype<state::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["change"]  = &state::SceneManager::change;
			scenemanager_type["clear"]   = &state::SceneManager::clear;
			scenemanager_type["load"]    = &state::SceneManager::load;
			scenemanager_type["save"]    = &state::SceneManager::save;
			scenemanager_type["current"] = &state::SceneManager::current;
			scenemanager_type["get"]     = &state::SceneManager::get;
			scenemanager_type["make"]    = &state::SceneManager::make;
			scenemanager_type["set"]     = &state::SceneManager::set;

			// Use scenemanager to create.
			auto scene_type          = lua.new_usertype<state::Scene>("Scene", sol::no_constructor);
			scene_type["set_name"]   = &state::Scene::set_name;
			scene_type["get_name"]   = &state::Scene::get_name;
			scene_type["layers"]     = &state::Scene::layers;
			scene_type["get_camera"] = &state::Scene::get_camera;

			lua["galaxy_state_manager"] = std::ref(core::ServiceLocator<state::SceneManager>::ref());

			/* SCRIPTING */
			auto basicscript_type =
				lua.new_usertype<lua::BasicScript>("BasicScript", sol::constructors<lua::BasicScript(), lua::BasicScript(std::string_view)>());
			basicscript_type["load"]           = &lua::BasicScript::load;
			basicscript_type["run"]            = &lua::BasicScript::run;
			basicscript_type["run_and_return"] = &lua::BasicScript::run_and_return;

			/* UTILS */
			auto guid_type         = lua.new_usertype<utils::Guid>("Guid", sol::constructors<utils::Guid()>());
			guid_type["as_string"] = &utils::Guid::to_string;
			guid_type["is_empty"]  = &utils::Guid::is_empty;

			lua.set("GALAXY_UPS", GALAXY_UPS);
			lua.set("GALAXY_DT", GALAXY_DT);
			lua.set("GALAXY_FLAG_BITSET_COUNT", GALAXY_FLAG_BITSET_COUNT);
			lua.set("GALAXY_VERTEX_EXT", GALAXY_VERTEX_EXT);
			lua.set("GALAXY_FRAGMENT_EXT", GALAXY_FRAGMENT_EXT);
			lua.set("GALAXY_IDENTITY_MATRIX", GALAXY_IDENTITY_MATRIX);
			lua.set("GAlAXY_CAMERA_UBO_INDEX", GAlAXY_CAMERA_UBO_INDEX);
			lua.set("GALAXY_MIN_CAMERA_ZOOM", GALAXY_MIN_CAMERA_ZOOM);
			lua.set("GALAXY_MAX_CAMERA_ZOOM", GALAXY_MAX_CAMERA_ZOOM);
			lua.set("GALAXY_DEFAULT_RENDERER_RESERVED", GALAXY_DEFAULT_RENDERER_RESERVED);
			lua.set("GALAXY_FONT_MSDF_RANGE", GALAXY_FONT_MSDF_RANGE);
			lua.set("GALAXY_FONT_MSDF_SCALE", GALAXY_FONT_MSDF_SCALE);
			lua.set("GALAXY_DEFAULT_ELLIPSE_FRAGMENTS", GALAXY_DEFAULT_ELLIPSE_FRAGMENTS);
			lua.set("GALAXY_REMOVE_LOG_FILES_OLDER_THAN_HOURS", GALAXY_REMOVE_LOG_FILES_OLDER_THAN_HOURS);
			lua.set("GALAXY_APP_DIR", GALAXY_APP_DIR);

			lua.set_function("str_split", &strutils::split);
			lua.set_function("str_parse_codepoint", &strutils::parse_codepoint);
			lua.set_function("str_begins_with", &strutils::begins_with);
		}

		void inject_services_into_lua()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua["galaxy_language"]     = std::ref(core::ServiceLocator<resource::Language>::ref());
			lua["galaxy_maps"]         = std::ref(core::ServiceLocator<resource::Maps>::ref());
			lua["galaxy_shaders"]      = std::ref(core::ServiceLocator<resource::Shaders>::ref());
			lua["galaxy_fonts"]        = std::ref(core::ServiceLocator<resource::Fonts>::ref());
			lua["galaxy_sounds"]       = std::ref(core::ServiceLocator<resource::Sounds>::ref());
			lua["galaxy_textureatlas"] = std::ref(core::ServiceLocator<resource::TextureAtlas>::ref());
			lua["galaxy_prefabs"]      = std::ref(core::ServiceLocator<resource::Prefabs>::ref());
		}
	} // namespace lua
} // namespace galaxy