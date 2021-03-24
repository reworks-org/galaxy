///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>

#include <sol/sol.hpp>

#include "galaxy/audio/Context.hpp"

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/algorithm/Random.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "galaxy/graphics/animation/Animation.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/Shader.hpp"

#include "galaxy/map/TiledWorld.hpp"

#include "galaxy/physics/AABB.hpp"
#include "galaxy/physics/BodyType.hpp"
#include "galaxy/physics/SATObject.hpp"

#include "galaxy/scripting/JSONUtils.hpp"

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/res/TextureAtlas.hpp"
#include "galaxy/res/SoundBook.hpp"
#include "galaxy/res/MusicBook.hpp"
#include "galaxy/res/ScriptBook.hpp"

#include "LuaUtils.hpp"

// BEGIN GENERIC FUNCTIONS

void log_wrapper(int error_level, std::string_view message)
{
	switch (error_level)
	{
		case 0:
			GALAXY_LOG(GALAXY_INFO, "{0}", message);
			break;

		case 1:
			GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
			break;

		case 2:
			GALAXY_LOG(GALAXY_WARNING, "{0}", message);
			break;

		case 3:
			GALAXY_LOG(GALAXY_ERROR, "{0}", message);
			break;

		case 4:
			GALAXY_LOG(GALAXY_FATAL, "{0}", message);
			break;

		default:
			GALAXY_LOG(GALAXY_INFO, "{0}", message);
			break;
	}
}

nlohmann::json parse_json(std::string_view json)
{
	return nlohmann::json::parse(json);
}

// BEGIN SHADER FUNCTIONS

void set_int_uniform(galaxy::graphics::Shader& shader, std::string_view name, const int val)
{
	shader.set_uniform("name", val);
}

void set_float_uniform(galaxy::graphics::Shader& shader, std::string_view name, const float val)
{
	shader.set_uniform("name", val);
}

void set_uint_uniform(galaxy::graphics::Shader& shader, std::string_view name, const unsigned int val)
{
	shader.set_uniform("name", val);
}

void set_vec2_uniform(galaxy::graphics::Shader& shader, std::string_view name, const glm::vec2& val)
{
	shader.set_uniform("name", val);
}

void set_vec3_uniform(galaxy::graphics::Shader& shader, std::string_view name, const glm::vec3& val)
{
	shader.set_uniform("name", val);
}

void set_vec4_uniform(galaxy::graphics::Shader& shader, std::string_view name, const glm::vec4& val)
{
	shader.set_uniform("name", val);
}

namespace galaxy
{
	namespace lua
	{
		void register_functions()
		{
			auto lua = SL_HANDLE.lua();

			// Algorithms.
			lua->set_function("galaxy_random_int", &algorithm::random<int>);
			lua->set_function("galaxy_random_float", &algorithm::random<float>);
			lua->set_function("galaxy_normalize", &algorithm::normalize<float>);

			// Decoding.
			lua->set_function("galaxy_decode_base64", &algorithm::decode_base64);
			lua->set_function("galaxy_decode_zlib", &algorithm::decode_zlib);
			lua->set_function("galaxy_decode_gzip", &algorithm::decode_gzip);

			// Error handling.
			lua->set_function("galaxy_log", &log_wrapper);
		}

		void register_audio()
		{
			auto lua = SL_HANDLE.lua();

			auto context_type                        = lua->new_usertype<audio::Context>("gContext", sol::no_constructor);
			context_type["set_doppler_factor"]       = &audio::Context::set_doppler_factor;
			context_type["set_listener_gain"]        = &audio::Context::set_listener_gain;
			context_type["set_listener_orientation"] = &audio::Context::set_listener_orientation;
			context_type["set_listener_position"]    = &audio::Context::set_listener_position;
			context_type["set_listener_velocity"]    = &audio::Context::set_listener_velocity;
			context_type["set_speed_of_sound"]       = &audio::Context::set_speed_of_sound;

			auto music_type                  = lua->new_usertype<audio::Music>("gMusic", sol::constructors<audio::Music()>());
			music_type["load"]               = &audio::Music::load;
			music_type["play"]               = &audio::Music::play;
			music_type["pause"]              = &audio::Music::pause;
			music_type["stop"]               = &audio::Music::stop;
			music_type["set_looping"]        = &audio::Music::set_looping;
			music_type["set_cone"]           = &audio::Music::set_cone;
			music_type["set_direction"]      = &audio::Music::set_direction;
			music_type["set_gain"]           = &audio::Music::set_gain;
			music_type["set_max_distance"]   = &audio::Music::set_max_distance;
			music_type["set_pitch"]          = &audio::Music::set_pitch;
			music_type["set_position"]       = &audio::Music::set_position;
			music_type["set_rolloff_factor"] = &audio::Music::set_rolloff_factor;
			music_type["set_velocity"]       = &audio::Music::set_velocity;

			auto sound_type                  = lua->new_usertype<audio::Sound>("gSound", sol::constructors<audio::Sound()>());
			sound_type["load"]               = &audio::Sound::load;
			sound_type["play"]               = &audio::Sound::play;
			sound_type["pause"]              = &audio::Sound::pause;
			sound_type["stop"]               = &audio::Sound::stop;
			sound_type["set_looping"]        = &audio::Sound::set_looping;
			sound_type["set_cone"]           = &audio::Sound::set_cone;
			sound_type["set_direction"]      = &audio::Sound::set_direction;
			sound_type["set_gain"]           = &audio::Sound::set_gain;
			sound_type["set_max_distance"]   = &audio::Sound::set_max_distance;
			sound_type["set_pitch"]          = &audio::Sound::set_pitch;
			sound_type["set_position"]       = &audio::Sound::set_position;
			sound_type["set_rolloff_factor"] = &audio::Sound::set_rolloff_factor;
			sound_type["set_velocity"]       = &audio::Sound::set_velocity;
		}

		void register_ecs()
		{
			auto lua = SL_HANDLE.lua();

			auto world_type                = lua->new_usertype<core::World>("gWorld", sol::no_constructor);
			world_type["create"]           = sol::resolve<const ecs::Entity(void)>(&core::World::create);
			world_type["create_with_name"] = &core::World::create;
			world_type["create_from_json"] = &core::World::create_from_json;
			world_type["destroy"]          = &core::World::destroy;
			world_type["has"]              = &core::World::has;
			world_type["is_enabled"]       = &core::World::is_enabled;
			world_type["enable"]           = &core::World::enable;
			world_type["disable"]          = &core::World::disable;
			world_type["clear"]            = &core::World::clear;
		}

		void register_events()
		{
			auto lua = SL_HANDLE.lua();

			// clang-format off
			lua->new_enum<input::MouseButton>("gMouseButton",
			{
				{"BUTTON_1", input::MouseButton::BUTTON_1},
				{"BUTTON_2", input::MouseButton::BUTTON_2},
				{"BUTTON_3", input::MouseButton::BUTTON_3},
				{"BUTTON_4", input::MouseButton::BUTTON_4},
				{"BUTTON_5", input::MouseButton::BUTTON_5},
				{"BUTTON_6", input::MouseButton::BUTTON_6},
				{"BUTTON_7", input::MouseButton::BUTTON_7},
				{"BUTTON_8", input::MouseButton::BUTTON_8},
				{"BUTTON_LAST", input::MouseButton::BUTTON_LAST},
				{"BUTTON_LEFT", input::MouseButton::BUTTON_LEFT},
				{"BUTTON_RIGHT", input::MouseButton::BUTTON_RIGHT},
				{"BUTTON_MIDDLE", input::MouseButton::BUTTON_MIDDLE}
			});

			lua->new_enum<input::Keys>("gKeys",
			{
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
				{"NUM_1", input::Keys::NUM_1},
				{"NUM_2", input::Keys::NUM_2},
				{"NUM_3", input::Keys::NUM_3},
				{"NUM_4", input::Keys::NUM_4},
				{"NUM_5", input::Keys::NUM_5},
				{"NUM_6", input::Keys::NUM_6},
				{"NUM_7", input::Keys::NUM_7},
				{"NUM_8", input::Keys::NUM_8},
				{"NUM_9", input::Keys::NUM_9},
				{"NUM_0", input::Keys::NUM_0},
				{"MINUS", input::Keys::MINUS},
				{"EQUALS", input::Keys::EQUALS},
				{"BACKSPACE", input::Keys::BACKSPACE},
				{"GRAVE", input::Keys::GRAVE},
				{"TAB", input::Keys::TAB},
				{"CAPS", input::Keys::CAPS},
				{"LSHIFT", input::Keys::LSHIFT},
				{"LCNTRL", input::Keys::LCNTRL},
				{"LSTART", input::Keys::LSTART},
				{"LALT", input::Keys::LALT},
				{"SPACE", input::Keys::SPACE},
				{"RALT", input::Keys::RALT},
				{"RSTART", input::Keys::RSTART},
				{"MENU", input::Keys::MENU},
				{"RCNTRL", input::Keys::RCNTRL},
				{"RSHIFT", input::Keys::RSHIFT},
				{"ENTER", input::Keys::ENTER},
				{"SEMICOLON", input::Keys::SEMICOLON},
				{"APOSTROPHE", input::Keys::APOSTROPHE},
				{"SLASH", input::Keys::SLASH},
				{"PERIOD", input::Keys::PERIOD},
				{"COMMA", input::Keys::COMMA},
				{"LBRACKET", input::Keys::LBRACKET},
				{"RBRACKET", input::Keys::RBRACKET},
				{"BACKSLASH", input::Keys::BACKSLASH},
				{"ESC", input::Keys::ESC},
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
				{"PRINTSCREEN", input::Keys::PRINTSCREEN},
				{"SCROLL_LOCK", input::Keys::SCROLL_LOCK},
				{"PAUSE", input::Keys::PAUSE},
				{"INSERT", input::Keys::INSERT},
				{"HOME", input::Keys::HOME},
				{"PAGEUP", input::Keys::PAGEUP},
				{"PAGEDOWN", input::Keys::PAGEDOWN},
				{"END", input::Keys::END},
				{"DEL", input::Keys::DEL},
				{"UP", input::Keys::UP},
				{"DOWN", input::Keys::DOWN},
				{"LEFT", input::Keys::LEFT},
				{"RIGHT", input::Keys::RIGHT},
				{"NUMLOCK", input::Keys::NUMLOCK},
				{"NUMPAD_MULTIPLY", input::Keys::NUMPAD_MULTIPLY},
				{"NUMPAD_DIVIDE", input::Keys::NUMPAD_DIVIDE},
				{"NUMPAD_ADD", input::Keys::NUMPAD_ADD},
				{"NUMPAD_ENTER", input::Keys::NUMPAD_ENTER},
				{"NUMPAD_PERIOD", input::Keys::NUMPAD_PERIOD},
				{"NUMPAD_0", input::Keys::NUMPAD_0},
				{"NUMPAD_1", input::Keys::NUMPAD_1},
				{"NUMPAD_2", input::Keys::NUMPAD_2},
				{"NUMPAD_3", input::Keys::NUMPAD_3},
				{"NUMPAD_4", input::Keys::NUMPAD_4},
				{"NUMPAD_5", input::Keys::NUMPAD_5},
				{"NUMPAD_6", input::Keys::NUMPAD_6},
				{"NUMPAD_7", input::Keys::NUMPAD_7},
				{"NUMPAD_8", input::Keys::NUMPAD_8},
				{"NUMPAD_9", input::Keys::NUMPAD_9}
			});
			// clang-format on
			auto key_down_type       = lua->new_usertype<events::KeyDown>("gKeyDown", sol::constructors<events::KeyDown()>());
			key_down_type["keycode"] = &events::KeyDown::m_keycode;

			auto key_repeat_type       = lua->new_usertype<events::KeyRepeat>("gKeyRepeat", sol::constructors<events::KeyRepeat()>());
			key_repeat_type["keycode"] = &events::KeyRepeat::m_keycode;

			auto key_up_type       = lua->new_usertype<events::KeyUp>("gKeyUp", sol::constructors<events::KeyUp()>());
			key_up_type["keycode"] = &events::KeyUp::m_keycode;

			auto mouse_moved_type = lua->new_usertype<events::MouseMoved>("gMouseMoved", sol::constructors<events::MouseMoved()>());
			mouse_moved_type["x"] = &events::MouseMoved::m_x;
			mouse_moved_type["y"] = &events::MouseMoved::m_y;

			auto mouse_pressed_type      = lua->new_usertype<events::MousePressed>("gMousePressed", sol::constructors<events::MousePressed()>());
			mouse_pressed_type["button"] = &events::MousePressed::m_button;
			mouse_pressed_type["x"]      = &events::MousePressed::m_x;
			mouse_pressed_type["y"]      = &events::MousePressed::m_y;

			auto mouse_released_type      = lua->new_usertype<events::MouseReleased>("gMouseReleased", sol::constructors<events::MouseReleased()>());
			mouse_released_type["button"] = &events::MouseReleased::m_button;
			mouse_released_type["x"]      = &events::MouseReleased::m_x;
			mouse_released_type["y"]      = &events::MouseReleased::m_y;

			auto mouse_wheel_type        = lua->new_usertype<events::MouseWheel>("gMouseWheel", sol::constructors<events::MouseWheel()>());
			mouse_wheel_type["x_offset"] = &events::MouseWheel::m_x_offset;
			mouse_wheel_type["y_offset"] = &events::MouseWheel::m_y_offset;

			auto window_resized_type      = lua->new_usertype<events::WindowResized>("gWindowResized", sol::constructors<events::WindowResized()>());
			window_resized_type["height"] = &events::WindowResized::m_height;
			window_resized_type["width"]  = &events::WindowResized::m_width;

			auto dispatcher_type                      = lua->new_usertype<events::Dispatcher>("gDispatcher", sol::constructors<events::Dispatcher()>());
			dispatcher_type["trigger_keydown"]        = &events::Dispatcher::trigger<events::KeyDown, const input::Keys>;
			dispatcher_type["trigger_keyrepeat"]      = &events::Dispatcher::trigger<events::KeyRepeat, const input::Keys>;
			dispatcher_type["trigger_keyup"]          = &events::Dispatcher::trigger<events::KeyUp, const input::Keys>;
			dispatcher_type["trigger_mouse_moved"]    = &events::Dispatcher::trigger<events::MouseMoved, const double, const double>;
			dispatcher_type["trigger_mouse_pressed"]  = &events::Dispatcher::trigger<events::MousePressed, const float, const float, const input::MouseButton>;
			dispatcher_type["trigger_mouse_released"] = &events::Dispatcher::trigger<events::MouseReleased, const float, const float, const input::MouseButton>;
			dispatcher_type["trigger_mouse_wheel"]    = &events::Dispatcher::trigger<events::MouseWheel, const double, const double>;
		}

		void register_fs()
		{
			auto lua = SL_HANDLE.lua();

			auto config_type        = lua->new_usertype<fs::Config>("gConfig", sol::no_constructor);
			config_type["init"]     = &fs::Config::init;
			config_type["save"]     = &fs::Config::save;
			config_type["is_blank"] = &fs::Config::is_blank;

			config_type["define_int"]    = &fs::Config::define<int>;
			config_type["define_bool"]   = &fs::Config::define<bool>;
			config_type["define_float"]  = &fs::Config::define<float>;
			config_type["define_string"] = &fs::Config::define<std::string>;

			config_type["change_int"]    = &fs::Config::change<int>;
			config_type["change_bool"]   = &fs::Config::change<bool>;
			config_type["change_float"]  = &fs::Config::change<float>;
			config_type["change_string"] = &fs::Config::change<std::string>;

			config_type["get_int"]    = &fs::Config::get<int>;
			config_type["get_bool"]   = &fs::Config::get<bool>;
			config_type["get_float"]  = &fs::Config::get<float>;
			config_type["get_string"] = &fs::Config::get<std::string>;

			auto fs_type           = lua->new_usertype<fs::Virtual>("gVirtualFS", sol::no_constructor);
			fs_type["open"]        = &fs::Virtual::open;
			fs_type["save"]        = &fs::Virtual::save;
			fs_type["absolute"]    = &fs::Virtual::absolute;
			fs_type["create_file"] = &fs::Virtual::create_file;
		}

		void register_graphics()
		{
			auto lua = SL_HANDLE.lua();

			// clang-format off
			lua->new_enum<graphics::Renderables>("gRenderables",
			{
				{"POINT", graphics::Renderables::POINT},
				{"LINE", graphics::Renderables::LINE},
				{"LINE_LOOP", graphics::Renderables::LINE_LOOP},
				{"BATCHED", graphics::Renderables::BATCHED},
				{"TEXT", graphics::Renderables::TEXT},
				{"SPRITE", graphics::Renderables::SPRITE}
			});

			lua->new_enum<graphics::Primitives>("gPrimitives",
			{
				{"CIRCLE", graphics::Primitives::CIRCLE},
				{"ELLIPSE", graphics::Primitives::ELLIPSE},
				{"LINE", graphics::Primitives::LINE},
				{"POINT", graphics::Primitives::POINT},
				{"POLYLINE", graphics::Primitives::POLYLINE},
				{"POLYGON", graphics::Primitives::POLYGON}
			});
			// clang-format on

			auto colour_type        = lua->new_usertype<graphics::Colour>("gColour", sol::constructors<graphics::Colour(), graphics::Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
			colour_type["r"]        = &graphics::Colour::m_red;
			colour_type["g"]        = &graphics::Colour::m_green;
			colour_type["b"]        = &graphics::Colour::m_blue;
			colour_type["a"]        = &graphics::Colour::m_alpha;
			colour_type["r_normal"] = &graphics::Colour::r_normal;
			colour_type["g_normal"] = &graphics::Colour::g_normal;
			colour_type["b_normal"] = &graphics::Colour::b_normal;
			colour_type["a_normal"] = &graphics::Colour::a_normal;

			auto irect_type              = lua->new_usertype<graphics::iRect>("giRect", sol::constructors<graphics::iRect(), graphics::iRect(int, int, int, int)>());
			irect_type["contains_point"] = sol::resolve<const bool(const int, const int)>(&graphics::iRect::contains);
			irect_type["contains_rect"]  = sol::resolve<const bool(const graphics::iRect&)>(&graphics::iRect::contains);
			irect_type["overlaps"]       = &graphics::iRect::overlaps;
			irect_type["x"]              = &graphics::iRect::m_x;
			irect_type["y"]              = &graphics::iRect::m_y;
			irect_type["w"]              = &graphics::iRect::m_width;
			irect_type["h"]              = &graphics::iRect::m_height;

			auto frect_type              = lua->new_usertype<graphics::fRect>("gfRect", sol::constructors<graphics::fRect(), graphics::fRect(float, float, float, float)>());
			frect_type["contains_point"] = sol::resolve<const bool(const float, const float)>(&graphics::fRect::contains);
			frect_type["contains_rect"]  = sol::resolve<const bool(const graphics::fRect&)>(&graphics::fRect::contains);
			frect_type["overlaps"]       = &graphics::fRect::overlaps;
			frect_type["x"]              = &graphics::fRect::m_x;
			frect_type["y"]              = &graphics::fRect::m_y;
			frect_type["w"]              = &graphics::fRect::m_width;
			frect_type["h"]              = &graphics::fRect::m_height;

			auto shader_type         = lua->new_usertype<graphics::Shader>("gShader", sol::constructors<graphics::Shader()>());
			shader_type["load_path"] = &graphics::Shader::load_path;
			shader_type["load_raw"]  = &graphics::Shader::load_raw;
			shader_type["bind"]      = &graphics::Shader::bind;
			shader_type["unbind"]    = &graphics::Shader::unbind;
			shader_type["is_loaded"] = &graphics::Shader::is_loaded;

			lua->set_function("gShader_set_int", &set_int_uniform);
			lua->set_function("gShader_set_float", &set_float_uniform);
			lua->set_function("gShader_set_uint", &set_uint_uniform);
			lua->set_function("gShader_set_vec2", &set_vec2_uniform);
			lua->set_function("gShader_set_vec3", &set_vec3_uniform);
			lua->set_function("gShader_set_vec4", &set_vec4_uniform);

			auto camera_type             = lua->new_usertype<graphics::Camera>("gCamera", sol::constructors<graphics::Camera(), graphics::Camera(const float, const float, const float, const float, const float)>());
			camera_type["create"]        = &graphics::Camera::create;
			camera_type["get_height"]    = &graphics::Camera::get_height;
			camera_type["get_pos"]       = &graphics::Camera::get_pos;
			camera_type["get_scale"]     = &graphics::Camera::get_scale;
			camera_type["get_speed"]     = &graphics::Camera::get_speed;
			camera_type["get_width"]     = &graphics::Camera::get_width;
			camera_type["is_dirty"]      = &graphics::Camera::is_dirty;
			camera_type["move"]          = &graphics::Camera::move;
			camera_type["zoom"]          = &graphics::Camera::zoom;
			camera_type["set_pos"]       = &graphics::Camera::set_pos;
			camera_type["set_speed"]     = &graphics::Camera::set_speed;
			camera_type["update"]        = &graphics::Camera::update;
			camera_type["get_proj"]      = &graphics::Camera::get_proj;
			camera_type["get_transform"] = &graphics::Camera::get_view;
			camera_type["move_x"]        = &graphics::Camera::move_x;
			camera_type["move_y"]        = &graphics::Camera::move_y;
			camera_type["set_width"]     = &graphics::Camera::set_width;
			camera_type["set_height"]    = &graphics::Camera::set_height;

			auto frame_type              = lua->new_usertype<graphics::Frame>("gFrame", sol::constructors<graphics::Frame(), graphics::Frame(const graphics::fRect&, const double)>());
			frame_type["set_region"]     = &graphics::Frame::set_region;
			frame_type["get_region"]     = &graphics::Frame::get_region;
			frame_type["get_region_id"]  = &graphics::Frame::get_region_id;
			frame_type["time_per_frame"] = &graphics::Frame::m_time_per_frame;

			auto animation_type                 = lua->new_usertype<graphics::Animation>("gAnimation", sol::no_constructor);
			animation_type["get_current_frame"] = &graphics::Animation::get_current_frame;
			animation_type["get_name"]          = &graphics::Animation::get_name;
			animation_type["get_speed"]         = &graphics::Animation::get_speed;
			animation_type["get_total_frames"]  = &graphics::Animation::get_total_frames;
			animation_type["is_looping"]        = &graphics::Animation::is_looping;
			animation_type["next_frame"]        = &graphics::Animation::next_frame;
			animation_type["restart"]           = &graphics::Animation::restart;
		}

		void register_json()
		{
			auto lua = SL_HANDLE.lua();

			auto json_type = lua->new_usertype<nlohmann::json>("gJSON", sol::constructors<nlohmann::json()>());
			lua->set_function("parse_json_from_str", &parse_json);
			lua->set_function("parse_json_from_disk", &json::parse_from_disk);
			lua->set_function("parse_json_from_mem", &json::parse_from_mem);
			lua->set_function("save_json_to_disk", &json::save_to_disk);
		}

		void register_mapping()
		{
			auto lua = SL_HANDLE.lua();

			auto map_type               = lua->new_usertype<map::Map>("gMap", sol::constructors<map::Map(), map::Map(std::string_view)>());
			map_type["load"]            = &map::Map::load;
			map_type["parse"]           = &map::Map::parse;
			map_type["create"]          = &map::Map::create;
			map_type["enable_objects"]  = &map::Map::enable_objects;
			map_type["disable_objects"] = &map::Map::disable_objects;

			auto world_map_type       = lua->new_usertype<map::TiledWorld>("gTiledWorld", sol::constructors<map::TiledWorld()>());
			world_map_type["load"]    = &map::TiledWorld::load;
			world_map_type["parse"]   = &map::TiledWorld::parse;
			world_map_type["get_map"] = &map::TiledWorld::get_map;
		}

		void register_math()
		{
			auto lua = SL_HANDLE.lua();

			auto vec2_type = lua->new_usertype<glm::vec2>("gVec2", sol::constructors<glm::vec2(), glm::vec2(float, float)>());
			vec2_type["x"] = &glm::vec2::x;
			vec2_type["y"] = &glm::vec2::y;

			auto vec3_type = lua->new_usertype<glm::vec3>("gVec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>());
			vec3_type["x"] = &glm::vec3::x;
			vec3_type["y"] = &glm::vec3::y;
			vec3_type["z"] = &glm::vec3::z;

			auto vec4_type = lua->new_usertype<glm::vec4>("gVec4", sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>());
			vec4_type["x"] = &glm::vec4::x;
			vec4_type["y"] = &glm::vec4::y;
			vec4_type["z"] = &glm::vec4::z;
			vec4_type["w"] = &glm::vec4::w;
		}

		void register_physics()
		{
			auto lua = SL_HANDLE.lua();

			// clang-format off
			lua->new_enum<physics::BodyType>("gBodyType",
			{
				{"DYNAMIC", physics::BodyType::DYNAMIC},
				{"STATIC", physics::BodyType::STATIC}
			});
			// clang-format on

			auto aabb_type            = lua->new_usertype<physics::AABB>("gAABB", sol::constructors<physics::AABB(), physics::AABB(const glm::vec2&, const glm::vec2&)>());
			aabb_type["area"]         = &physics::AABB::area;
			aabb_type["compute_area"] = &physics::AABB::compute_area;
			aabb_type["contains"]     = &physics::AABB::contains;
			aabb_type["fatten"]       = &physics::AABB::fatten;
			aabb_type["max"]          = &physics::AABB::max;
			aabb_type["min"]          = &physics::AABB::min;
			aabb_type["overlaps"]     = &physics::AABB::overlaps;
			aabb_type["size"]         = &physics::AABB::size;
			aabb_type["update_area"]  = &physics::AABB::update_area;
			lua->set_function("galaxy_merge_aabbs", &physics::AABB::merge);
		}

		void register_res()
		{
			auto lua = SL_HANDLE.lua();

			auto shaderbook_type                = lua->new_usertype<res::ShaderBook>("gShaderBook", sol::no_constructor);
			shaderbook_type["create_from_json"] = &res::ShaderBook::create_from_json;
			shaderbook_type["get"]              = &res::ShaderBook::get;
			shaderbook_type["clear"]            = &res::ShaderBook::clear;

			auto fontbook_type                = lua->new_usertype<res::FontBook>("gFontBook", sol::no_constructor);
			fontbook_type["create_from_json"] = &res::FontBook::create_from_json;
			fontbook_type["get"]              = &res::FontBook::get;
			fontbook_type["clear"]            = &res::FontBook::clear;

			auto tex_atlas_type                 = lua->new_usertype<res::TextureAtlas>("gTextureAtlas", sol::no_constructor);
			tex_atlas_type["add"]               = &res::TextureAtlas::add;
			tex_atlas_type["create"]            = &res::TextureAtlas::create;
			tex_atlas_type["get_size"]          = &res::TextureAtlas::get_size;
			tex_atlas_type["get_region"]        = &res::TextureAtlas::get_region;
			tex_atlas_type["save"]              = &res::TextureAtlas::save;
			tex_atlas_type["update"]            = &res::TextureAtlas::update;
			tex_atlas_type["add_custom_region"] = &res::TextureAtlas::add_custom_region;

			auto soundbook_type                = lua->new_usertype<res::SoundBook>("gSoundBook", sol::no_constructor);
			soundbook_type["create_from_json"] = &res::SoundBook::create_from_json;
			soundbook_type["get"]              = &res::SoundBook::get;
			soundbook_type["clear"]            = &res::SoundBook::clear;

			auto musicbook_type                = lua->new_usertype<res::MusicBook>("gMusicBook", sol::no_constructor);
			musicbook_type["create_from_json"] = &res::MusicBook::create_from_json;
			musicbook_type["get"]              = &res::MusicBook::get;
			musicbook_type["clear"]            = &res::MusicBook::clear;

			auto scriptbook_type                = lua->new_usertype<res::ScriptBook>("gScriptBook", sol::no_constructor);
			scriptbook_type["create_from_json"] = &res::ScriptBook::create_from_json;
			scriptbook_type["clear"]            = &res::ScriptBook::clear;
			scriptbook_type["get"]              = &res::ScriptBook::get;
		}
	} // namespace lua
} // namespace galaxy