///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/algorithm/Random.hpp"

#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/core/LayerStack.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/shapes/Rect.hpp"
#include "galaxy/graphics/Shader.hpp"

#include "galaxy/scripting/JSONUtils.hpp"

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "LuaUtils.hpp"

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

galaxy::components::ShaderID* add_shaderid(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::ShaderID>(entity);
}

galaxy::components::Transform* add_transform(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Transform>(entity);
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

			/*
			auto music_type           = lua->new_usertype<audio::Music>("Music", sol::constructors<audio::Music()>());
			music_type["load"]        = &audio::Music::load;
			music_type["play"]        = &audio::Music::play;
			music_type["pause"]       = &audio::Music::pause;
			music_type["stop"]        = &audio::Music::stop;
			music_type["rewind"]      = &audio::Music::rewind;
			music_type["set_looping"] = &audio::Music::set_looping;

			auto sound_type           = lua->new_usertype<audio::Sound>("Sound", sol::constructors<audio::Sound()>());
			sound_type["load"]        = &audio::Sound::load;
			sound_type["play"]        = &audio::Sound::play;
			sound_type["pause"]       = &audio::Sound::pause;
			sound_type["stop"]        = &audio::Sound::stop;
			sound_type["rewind"]      = &audio::Sound::rewind;
			sound_type["set_looping"] = &audio::Sound::set_looping;

			auto music_playlist_type           = lua->new_usertype<audio::Playlist<audio::Music>>("MusicPlaylist", sol::constructors<audio::Playlist<audio::Music>()>());
			music_playlist_type["load"]        = &audio::Playlist<audio::Music>::add;
			music_playlist_type["play"]        = &audio::Playlist<audio::Music>::play;
			music_playlist_type["pause"]       = &audio::Playlist<audio::Music>::pause;
			music_playlist_type["stop"]        = &audio::Playlist<audio::Music>::stop;
			music_playlist_type["rewind"]      = &audio::Playlist<audio::Music>::shuffle;
			music_playlist_type["set_looping"] = &audio::Playlist<audio::Music>::toggle_loop;

			auto sound_playlist_type           = lua->new_usertype<audio::Playlist<audio::Sound>>("SoundPlaylist", sol::constructors<audio::Playlist<audio::Sound>()>());
			sound_playlist_type["load"]        = &audio::Playlist<audio::Sound>::add;
			sound_playlist_type["play"]        = &audio::Playlist<audio::Sound>::play;
			sound_playlist_type["pause"]       = &audio::Playlist<audio::Sound>::pause;
			sound_playlist_type["stop"]        = &audio::Playlist<audio::Sound>::stop;
			sound_playlist_type["rewind"]      = &audio::Playlist<audio::Sound>::shuffle;
			sound_playlist_type["set_looping"] = &audio::Playlist<audio::Sound>::toggle_loop;
			*/
		}

		void register_ecs()
		{
			auto lua = SL_HANDLE.lua();

			auto entity_type               = lua->new_usertype<ecs::Entity>("gEntity");
			auto world_type                = lua->new_usertype<core::World>("gWorld", sol::constructors<core::World()>());
			world_type["create"]           = sol::resolve<const ecs::Entity(void)>(&core::World::create);
			world_type["create_with_name"] = sol::resolve<const ecs::Entity(std::string_view)>(&core::World::create);
			world_type["create_from_json"] = &core::World::create_from_json;
			world_type["destroy"]          = &core::World::destroy;
			world_type["has"]              = &core::World::has;
			world_type["is_enabled"]       = &core::World::is_enabled;
			world_type["enable"]           = &core::World::enable;
			world_type["disable"]          = &core::World::disable;
			world_type["assign_name"]      = &core::World::assign_name;
			world_type["find_from_name"]   = &core::World::find_from_name;
			world_type["clear"]            = &core::World::clear;

			lua->set_function("add_shaderid_to_entity", &add_shaderid);
			lua->set_function("add_transform_to_entity", &add_transform);

			auto shaderid_type         = lua->new_usertype<components::ShaderID>("gShaderID", sol::constructors<components::ShaderID(), components::ShaderID(std::string_view)>());
			shaderid_type["shader_id"] = &components::ShaderID::m_shader_id;

			auto transform_type                   = lua->new_usertype<components::Transform>("gTransform", sol::constructors<components::Transform()>());
			transform_type["get_pos"]             = &components::Transform::get_pos;
			transform_type["get_rotation"]        = &components::Transform::get_rotation;
			transform_type["get_scale"]           = &components::Transform::get_scale;
			transform_type["is_dirty"]            = &components::Transform::is_dirty;
			transform_type["move"]                = &components::Transform::move;
			transform_type["rotate"]              = &components::Transform::rotate;
			transform_type["scale"]               = &components::Transform::scale;
			transform_type["set_pos"]             = &components::Transform::set_pos;
			transform_type["set_rotation_origin"] = &components::Transform::set_rotation_origin;
		}

		void register_fs()
		{
			auto lua = SL_HANDLE.lua();

			auto config_type        = lua->new_usertype<fs::Config>("gConfig", sol::constructors<fs::Config()>());
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

			auto fs_type        = lua->new_usertype<fs::Virtual>("gVirtualFS", sol::constructors<fs::Virtual()>());
			fs_type["mount"]    = sol::resolve<const bool(std::string_view)>(&fs::Virtual::mount);
			fs_type["open"]     = &fs::Virtual::open;
			fs_type["save"]     = &fs::Virtual::save;
			fs_type["absolute"] = &fs::Virtual::absolute;
		}

		void register_graphics()
		{
			auto lua = SL_HANDLE.lua();

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

			auto camera_type                   = lua->new_usertype<graphics::Camera>("gCamera", sol::constructors<graphics::Camera(), graphics::Camera(const float, const float, const float, const float)>());
			camera_type["create"]              = &graphics::Camera::create;
			camera_type["get_height"]          = &graphics::Camera::get_height;
			camera_type["get_pos"]             = &graphics::Camera::get_pos;
			camera_type["get_rotation"]        = &graphics::Camera::get_rotation;
			camera_type["get_scale"]           = &graphics::Camera::get_scale;
			camera_type["get_speed"]           = &graphics::Camera::get_speed;
			camera_type["get_width"]           = &graphics::Camera::get_width;
			camera_type["is_dirty"]            = &graphics::Camera::is_dirty;
			camera_type["move"]                = &graphics::Camera::move;
			camera_type["rotate"]              = &graphics::Camera::rotate;
			camera_type["scale"]               = &graphics::Camera::scale;
			camera_type["set_pos"]             = &graphics::Camera::set_pos;
			camera_type["set_rotation_origin"] = &graphics::Camera::set_rotation_origin;
			camera_type["set_speed"]           = &graphics::Camera::set_speed;
			camera_type["update"]              = &graphics::Camera::update;
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

		void register_layerstack()
		{
			auto lua = SL_HANDLE.lua();

			auto layerstack_type     = lua->new_usertype<core::LayerStack>("gLayerStack", sol::constructors<core::LayerStack()>());
			layerstack_type["push"]  = &core::LayerStack::push;
			layerstack_type["pop"]   = &core::LayerStack::pop;
			layerstack_type["clear"] = &core::LayerStack::clear;
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

		void register_res()
		{
			auto lua = SL_HANDLE.lua();

			auto shaderbook_type                = lua->new_usertype<res::ShaderBook>("gShaderBook", sol::constructors<res::ShaderBook(), res::ShaderBook(std::string_view)>());
			shaderbook_type["create_from_json"] = &res::ShaderBook::create_from_json;
			shaderbook_type["get"]              = &res::ShaderBook::get;
			shaderbook_type["clear"]            = &res::ShaderBook::clear;

			auto fontbook_type                = lua->new_usertype<res::FontBook>("gFontBook", sol::constructors<res::FontBook(), res::FontBook(std::string_view)>());
			fontbook_type["create_from_json"] = &res::FontBook::create_from_json;
			fontbook_type["get"]              = &res::FontBook::get;
			fontbook_type["clear"]            = &res::FontBook::clear;
		}
	} // namespace lua
} // namespace galaxy