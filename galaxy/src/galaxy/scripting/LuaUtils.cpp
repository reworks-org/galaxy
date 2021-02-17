///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/audio/Context.hpp"

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/algorithm/Random.hpp"

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Line.hpp"
#include "galaxy/components/OnEvent.hpp"
#include "galaxy/components/Physics.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/ShaderID.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/core/LayerStack.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"

#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/events/Collision.hpp"
#include "galaxy/events/FinishCollision.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/KeyDown.hpp"
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
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/particle/ParticleGenerator.hpp"

#include "galaxy/physics/Box2DIntegration.hpp"

#include "galaxy/scripting/JSONUtils.hpp"

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/res/TextureAtlas.hpp"
#include "galaxy/res/SoundBook.hpp"
#include "galaxy/res/MusicBook.hpp"

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

// BEGIN ADD COMPONENTS

galaxy::components::ShaderID* add_shaderid(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::ShaderID>(entity);
}

galaxy::components::Transform* add_transform(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Transform>(entity);
}

galaxy::components::Renderable* add_renderable(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Renderable>(entity);
}

galaxy::components::Point* add_point(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Point>(entity);
}

galaxy::components::Line* add_line(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Line>(entity);
}

galaxy::components::Circle* add_circle(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Circle>(entity);
}

galaxy::components::BatchedSprite* add_batched_sprite(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::BatchedSprite>(entity);
}

galaxy::components::Sprite* add_sprite(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Sprite>(entity);
}

galaxy::components::Text* add_text(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Text>(entity);
}

galaxy::components::Animated* add_animated(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Animated>(entity);
}

galaxy::components::Physics* add_physics(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Physics>(entity);
}

galaxy::components::Tag* add_tag(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::Tag>(entity);
}

// BEGIN GET COMPONENTS

galaxy::components::ShaderID* get_shaderid(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::ShaderID>(entity);
}

galaxy::components::Transform* get_transform(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Transform>(entity);
}

galaxy::components::Renderable* get_renderable(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Renderable>(entity);
}

galaxy::components::Point* get_point(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Point>(entity);
}

galaxy::components::Line* get_line(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Line>(entity);
}

galaxy::components::Circle* get_circle(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Circle>(entity);
}

galaxy::components::BatchedSprite* get_batched_sprite(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::BatchedSprite>(entity);
}

galaxy::components::Sprite* get_sprite(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Sprite>(entity);
}

galaxy::components::Text* get_text(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Text>(entity);
}

galaxy::components::Animated* get_animated(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Animated>(entity);
}

galaxy::components::Physics* get_physics(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Physics>(entity);
}

galaxy::components::Tag* get_tag(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::Tag>(entity);
}

// BEGIN EVENT ADD/GET

galaxy::components::OnEvent<galaxy::events::KeyChar>* add_keychar(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::KeyChar>>(entity);
}

galaxy::components::OnEvent<galaxy::events::KeyDown>* add_keydown(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::KeyDown>>(entity);
}

galaxy::components::OnEvent<galaxy::events::KeyUp>* add_keyup(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::KeyUp>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseMoved>* add_mouse_moved(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::MouseMoved>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MousePressed>* add_mouse_pressed(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::MousePressed>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseReleased>* add_mouse_released(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::MouseReleased>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseWheel>* add_mouse_wheel(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::MouseWheel>>(entity);
}

galaxy::components::OnEvent<galaxy::events::WindowResized>* add_window_resized(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::WindowResized>>(entity);
}

galaxy::components::OnEvent<galaxy::events::Collision>* add_on_collision(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::Collision>>(entity);
}

galaxy::components::OnEvent<galaxy::events::FinishCollision>* add_finish_collision(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.create_component<galaxy::components::OnEvent<galaxy::events::FinishCollision>>(entity);
}

galaxy::components::OnEvent<galaxy::events::KeyChar>* get_keychar(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::KeyChar>>(entity);
}

galaxy::components::OnEvent<galaxy::events::KeyDown>* get_keydown(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::KeyDown>>(entity);
}

galaxy::components::OnEvent<galaxy::events::KeyUp>* get_keyup(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::KeyUp>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseMoved>* get_mouse_moved(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::MouseMoved>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MousePressed>* get_mouse_pressed(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::MousePressed>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseReleased>* get_mouse_released(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::MouseReleased>>(entity);
}

galaxy::components::OnEvent<galaxy::events::MouseWheel>* get_mouse_wheel(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::MouseWheel>>(entity);
}

galaxy::components::OnEvent<galaxy::events::WindowResized>* get_window_resized(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::WindowResized>>(entity);
}

galaxy::components::OnEvent<galaxy::events::Collision>* get_on_collision(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::Collision>>(entity);
}

galaxy::components::OnEvent<galaxy::events::FinishCollision>* get_finish_collision(galaxy::core::World& world, const galaxy::ecs::Entity entity)
{
	return world.get<galaxy::components::OnEvent<galaxy::events::FinishCollision>>(entity);
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
			world_type["set_gravity"]      = sol::resolve<void(const float, const float)>(&core::World::set_gravity);

			lua->set_function("add_shaderid_to_entity", &add_shaderid);
			lua->set_function("add_transform_to_entity", &add_transform);
			lua->set_function("add_renderable_to_entity", &add_renderable);
			lua->set_function("add_point_to_entity", &add_point);
			lua->set_function("add_line_to_entity", &add_line);
			lua->set_function("add_circle_to_entity", &add_circle);
			lua->set_function("add_batched_sprite_to_entity", &add_batched_sprite);
			lua->set_function("add_sprite_to_entity", &add_sprite);
			lua->set_function("add_text_to_entity", &add_text);
			lua->set_function("add_animated_to_entity", &add_animated);
			lua->set_function("add_physics_to_entity", &add_physics);
			lua->set_function("add_tag_to_entity", &add_tag);

			lua->set_function("get_shaderid_from_entity", &get_shaderid);
			lua->set_function("get_transform_from_entity", &get_transform);
			lua->set_function("get_renderable_from_entity", &get_renderable);
			lua->set_function("get_point_from_entity", &get_point);
			lua->set_function("get_line_from_entity", &get_line);
			lua->set_function("get_circle_from_entity", &get_circle);
			lua->set_function("get_batched_sprite_from_entity", &get_batched_sprite);
			lua->set_function("get_sprite_from_entity", &get_sprite);
			lua->set_function("get_text_from_entity", &get_text);
			lua->set_function("get_animated_from_entity", &get_animated);
			lua->set_function("get_physics_from_entity", &get_physics);
			lua->set_function("get_tag_from_entity", &get_tag);

			lua->set_function("add_keychar_to_entity", &add_keychar);
			lua->set_function("add_keydown_to_entity", &add_keydown);
			lua->set_function("add_keyup_to_entity", &add_keyup);
			lua->set_function("add_mouse_moved_to_entity", &add_mouse_moved);
			lua->set_function("add_mouse_pressed_to_entity", &add_mouse_pressed);
			lua->set_function("add_mouse_released_to_entity", &add_mouse_released);
			lua->set_function("add_mouse_wheel_to_entity", &add_mouse_wheel);
			lua->set_function("add_window_resized_to_entity", &add_window_resized);
			lua->set_function("add_on_collision_to_entity", &add_on_collision);
			lua->set_function("add_finish_collision_to_entity", &add_finish_collision);
			lua->set_function("get_keychar_from_entity", &get_keychar);
			lua->set_function("get_keydown_from_entity", &get_keydown);
			lua->set_function("get_keyup_from_entity", &get_keyup);
			lua->set_function("get_mouse_moved_from_entity", &get_mouse_moved);
			lua->set_function("get_mouse_pressed_from_entity", &get_mouse_pressed);
			lua->set_function("get_mouse_released_from_entity", &get_mouse_released);
			lua->set_function("get_mouse_wheel_from_entity", &get_mouse_wheel);
			lua->set_function("get_window_resized_from_entity", &get_window_resized);
			lua->set_function("get_on_collision_from_entity", &get_on_collision);
			lua->set_function("get_finish_collision_from_entity", &get_finish_collision);

			auto shaderid_type         = lua->new_usertype<components::ShaderID>("gShaderID", sol::constructors<components::ShaderID(), components::ShaderID(std::string_view)>());
			shaderid_type["shader_id"] = &components::ShaderID::m_shader_id;

			auto transform_type                   = lua->new_usertype<components::Transform>("gTransform", sol::constructors<components::Transform()>());
			transform_type["get_pos"]             = &components::Transform::get_pos;
			transform_type["get_rotation"]        = &components::Transform::get_rotation;
			transform_type["is_dirty"]            = &components::Transform::is_dirty;
			transform_type["move"]                = &components::Transform::move;
			transform_type["rotate"]              = &components::Transform::rotate;
			transform_type["set_pos"]             = &components::Transform::set_pos;
			transform_type["set_rotation_origin"] = &components::Transform::set_rotation_origin;

			// clang-format off
			lua->new_enum<graphics::Renderables>("gRenderables",
			{
				{"POINT", graphics::Renderables::POINT},
				{"LINE", graphics::Renderables::LINE},
				{"CIRCLE", graphics::Renderables::CIRCLE},
				{"SPRITE", graphics::Renderables::SPRITE},
				{"TEXT", graphics::Renderables::TEXT},
				{"BATCHED", graphics::Renderables::BATCHED}
			});
			// clang-format on

			auto renderable_type       = lua->new_usertype<components::Renderable>("gRenderable", sol::constructors<components::Renderable(), components::Renderable(graphics::Renderables, const int)>());
			renderable_type["type"]    = &components::Renderable::m_type;
			renderable_type["z_level"] = &components::Renderable::m_z_level;

			auto point_type           = lua->new_usertype<components::Point>("gPoint", sol::constructors<components::Point(), components::Point(const unsigned int, graphics::Colour&)>());
			point_type["create"]      = &components::Point::create;
			point_type["bind"]        = &components::Point::bind;
			point_type["unbind"]      = &components::Point::unbind;
			point_type["get_size"]    = &components::Point::get_size;
			point_type["index_count"] = &components::Point::index_count;
			point_type["opacity"]     = &components::Point::opacity;
			point_type["set_opacity"] = &components::Point::set_opacity;

			auto line_type           = lua->new_usertype<components::Line>("gLine", sol::constructors<components::Line(), components::Line(graphics::Colour&, const float, const float, const float, const float)>());
			line_type["create"]      = &components::Line::create;
			line_type["bind"]        = &components::Line::bind;
			line_type["unbind"]      = &components::Line::unbind;
			line_type["index_count"] = &components::Line::index_count;
			line_type["opacity"]     = &components::Line::opacity;
			line_type["set_opacity"] = &components::Line::set_opacity;

			auto circle_type           = lua->new_usertype<components::Circle>("gCircle", sol::constructors<components::Circle(), components::Circle(const float, const unsigned int, graphics::Colour&)>());
			circle_type["create"]      = &components::Circle::create;
			circle_type["bind"]        = &components::Circle::bind;
			circle_type["unbind"]      = &components::Circle::unbind;
			circle_type["radius"]      = &components::Circle::radius;
			circle_type["index_count"] = &components::Circle::index_count;
			circle_type["opacity"]     = &components::Circle::opacity;
			circle_type["set_opacity"] = &components::Circle::set_opacity;

			auto bs_type                 = lua->new_usertype<components::BatchedSprite>("gBatchedSprite", sol::constructors<components::BatchedSprite()>());
			bs_type["create_region"]     = sol::resolve<void(const graphics::fRect&, float)>(&components::BatchedSprite::create);
			bs_type["create_atlas"]      = sol::resolve<void(std::string_view, float)>(&components::BatchedSprite::create);
			bs_type["get_height"]        = &components::BatchedSprite::get_height;
			bs_type["get_region"]        = &components::BatchedSprite::get_region;
			bs_type["get_width"]         = &components::BatchedSprite::get_width;
			bs_type["opacity"]           = &components::BatchedSprite::opacity;
			bs_type["set_opacity"]       = &components::BatchedSprite::set_opacity;
			bs_type["set_region"]        = &components::BatchedSprite::set_region;
			bs_type["set_custom_height"] = &components::BatchedSprite::set_custom_height;
			bs_type["set_custom_width"]  = &components::BatchedSprite::set_custom_width;

			auto sprite_type               = lua->new_usertype<components::Sprite>("gSprite", sol::constructors<components::Sprite()>());
			sprite_type["bind"]            = &components::Sprite::bind;
			sprite_type["unbind"]          = &components::Sprite::unbind;
			sprite_type["clamp_to_border"] = &components::Sprite::clamp_to_border;
			sprite_type["clamp_to_edge"]   = &components::Sprite::clamp_to_edge;
			sprite_type["stretch"]         = &components::Sprite::stretch;
			sprite_type["create"]          = &components::Sprite::create;
			sprite_type["get_aniso_level"] = &components::Sprite::get_aniso_level;
			sprite_type["get_height"]      = &components::Sprite::get_height;
			sprite_type["get_width"]       = &components::Sprite::get_width;
			sprite_type["opacity"]         = &components::Sprite::opacity;
			sprite_type["save"]            = &components::Sprite::save;
			sprite_type["set_anisotropy"]  = &components::Sprite::set_anisotropy;
			sprite_type["set_mirrored"]    = &components::Sprite::set_mirrored;
			sprite_type["set_opacity"]     = &components::Sprite::set_opacity;
			sprite_type["set_repeated"]    = &components::Sprite::set_repeated;

			auto text_type                = lua->new_usertype<components::Text>("gText", sol::constructors<components::Text>());
			text_type["bind"]             = &components::Text::bind;
			text_type["create"]           = &components::Text::create;
			text_type["get_batch_height"] = &components::Text::get_batch_height;
			text_type["get_batch_width"]  = &components::Text::get_batch_width;
			text_type["get_colour"]       = &components::Text::get_colour;
			text_type["get_height"]       = &components::Text::get_height;
			text_type["get_width"]        = &components::Text::get_width;
			text_type["load"]             = &components::Text::load;
			text_type["unbind"]           = &components::Text::unbind;
			text_type["update_text"]      = &components::Text::update_text;

			auto animated_type             = lua->new_usertype<components::Animated>("gAnimated", sol::constructors<components::Animated()>());
			animated_type["pause"]         = &components::Animated::pause;
			animated_type["play"]          = sol::resolve<void(void)>(&components::Animated::play);
			animated_type["play_specific"] = sol::resolve<void(std::string_view)>(&components::Animated::play);
			animated_type["set_animation"] = &components::Animated::set_animation;
			animated_type["stop"]          = &components::Animated::stop;
			animated_type["is_paused"]     = &components::Animated::is_paused;

			auto physics_type                = lua->new_usertype<components::Physics>("gPhysics", sol::constructors<components::Physics()>());
			physics_type["create_from_json"] = &components::Physics::create_from_json;
			physics_type["body"]             = &components::Physics::body;

			auto on_key_char_type        = lua->new_usertype<components::OnEvent<events::KeyChar>>("gOnKeyChar", sol::constructors<components::OnEvent<events::KeyChar>()>());
			on_key_char_type["on_event"] = &components::OnEvent<events::KeyChar>::m_on_event;

			auto on_key_down_type        = lua->new_usertype<components::OnEvent<events::KeyDown>>("gOnKeyDown", sol::constructors<components::OnEvent<events::KeyDown>()>());
			on_key_down_type["on_event"] = &components::OnEvent<events::KeyDown>::m_on_event;

			auto on_key_up_type        = lua->new_usertype<components::OnEvent<events::KeyUp>>("gOnKeyUp", sol::constructors<components::OnEvent<events::KeyUp>()>());
			on_key_up_type["on_event"] = &components::OnEvent<events::KeyUp>::m_on_event;

			auto on_mouse_moved_type        = lua->new_usertype<components::OnEvent<events::MouseMoved>>("gOnMouseMoved", sol::constructors<components::OnEvent<events::MouseMoved>()>());
			on_mouse_moved_type["on_event"] = &components::OnEvent<events::MouseMoved>::m_on_event;

			auto on_mouse_pressed_type        = lua->new_usertype<components::OnEvent<events::MousePressed>>("gOnMousePressed", sol::constructors<components::OnEvent<events::MousePressed>()>());
			on_mouse_pressed_type["on_event"] = &components::OnEvent<events::MousePressed>::m_on_event;

			auto on_mouse_released_type        = lua->new_usertype<components::OnEvent<events::MouseReleased>>("gOnMouseReleased", sol::constructors<components::OnEvent<events::MouseReleased>()>());
			on_mouse_released_type["on_event"] = &components::OnEvent<events::MouseReleased>::m_on_event;

			auto on_mouse_wheel_type        = lua->new_usertype<components::OnEvent<events::MouseWheel>>("gOnMouseWheel", sol::constructors<components::OnEvent<events::MouseWheel>()>());
			on_mouse_wheel_type["on_event"] = &components::OnEvent<events::MouseWheel>::m_on_event;

			auto on_window_resized_type        = lua->new_usertype<components::OnEvent<events::WindowResized>>("gOnWindowResized", sol::constructors<components::OnEvent<events::WindowResized>()>());
			on_window_resized_type["on_event"] = &components::OnEvent<events::WindowResized>::m_on_event;

			auto on_collision_type        = lua->new_usertype<components::OnEvent<events::Collision>>("gOnCollision", sol::constructors<components::OnEvent<events::Collision>()>());
			on_collision_type["on_event"] = &components::OnEvent<events::Collision>::m_on_event;

			auto finish_collision_type        = lua->new_usertype<components::OnEvent<events::FinishCollision>>("gFinishCollision", sol::constructors<components::OnEvent<events::FinishCollision>()>());
			finish_collision_type["on_event"] = &components::OnEvent<events::FinishCollision>::m_on_event;

			auto tag_type   = lua->new_usertype<components::Tag>("gTag", sol::constructors<components::Tag(), components::Tag(std::string_view)>());
			tag_type["tag"] = &components::Tag::m_tag;
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

			auto key_char_type       = lua->new_usertype<events::KeyChar>("gKeyChar", sol::constructors<events::KeyChar()>());
			key_char_type["keycode"] = &events::KeyChar::m_keycode;
			key_char_type["unichar"] = &events::KeyChar::m_unichar;

			auto key_down_type       = lua->new_usertype<events::KeyDown>("gKeyDown", sol::constructors<events::KeyDown()>());
			key_down_type["keycode"] = &events::KeyDown::m_keycode;

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

			auto collision_type = lua->new_usertype<events::Collision>("gCollision", sol::constructors<events::Collision(), events::Collision(b2Body*, b2Body*)>());
			collision_type["a"] = &events::Collision::m_a;
			collision_type["b"] = &events::Collision::m_b;

			auto finish_collision_type = lua->new_usertype<events::FinishCollision>("gFinishCollision", sol::constructors<events::FinishCollision(), events::FinishCollision(b2Body*, b2Body*)>());
			finish_collision_type["a"] = &events::FinishCollision::m_a;
			finish_collision_type["b"] = &events::FinishCollision::m_b;

			auto dispatcher_type                      = lua->new_usertype<events::Dispatcher>("gDispatcher", sol::constructors<events::Dispatcher()>());
			dispatcher_type["trigger_keychar"]        = &events::Dispatcher::trigger<events::KeyChar, const input::Keys, const int>;
			dispatcher_type["trigger_keydown"]        = &events::Dispatcher::trigger<events::KeyDown, const input::Keys>;
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

			auto fs_type        = lua->new_usertype<fs::Virtual>("gVirtualFS", sol::no_constructor);
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

			auto camera_type          = lua->new_usertype<graphics::Camera>("gCamera", sol::constructors<graphics::Camera(), graphics::Camera(const float, const float, const float, const float, const float)>());
			camera_type["create"]     = &graphics::Camera::create;
			camera_type["get_height"] = &graphics::Camera::get_height;
			camera_type["get_pos"]    = &graphics::Camera::get_pos;
			camera_type["get_scale"]  = &graphics::Camera::get_scale;
			camera_type["get_speed"]  = &graphics::Camera::get_speed;
			camera_type["get_width"]  = &graphics::Camera::get_width;
			camera_type["is_dirty"]   = &graphics::Camera::is_dirty;
			camera_type["move"]       = &graphics::Camera::move;
			camera_type["zoom"]       = &graphics::Camera::zoom;
			camera_type["set_pos"]    = &graphics::Camera::set_pos;
			camera_type["set_speed"]  = &graphics::Camera::set_speed;
			camera_type["update"]     = &graphics::Camera::update;

			auto particle_type            = lua->new_usertype<graphics::Particle>("gParticle", sol::constructors<graphics::Particle(), graphics::Particle(const float, const float)>());
			particle_type["angle"]        = &graphics::Particle::angle;
			particle_type["move"]         = &graphics::Particle::move;
			particle_type["life"]         = &graphics::Particle::m_life;
			particle_type["pos"]          = &graphics::Particle::pos;
			particle_type["set_position"] = &graphics::Particle::set_position;
			particle_type["set_velocity"] = &graphics::Particle::set_velocity;
			particle_type["velocity"]     = &graphics::Particle::velocity;

			auto particle_instance_type                = lua->new_usertype<graphics::ParticleInstance>("gParticleInstance", sol::constructors<>());
			particle_instance_type["clamp_to_border"]  = &graphics::ParticleInstance::clamp_to_border;
			particle_instance_type["clamp_to_edge"]    = &graphics::ParticleInstance::clamp_to_edge;
			particle_instance_type["create"]           = &graphics::ParticleInstance::create;
			particle_instance_type["get_aniso_level"]  = &graphics::ParticleInstance::get_aniso_level;
			particle_instance_type["get_height"]       = &graphics::ParticleInstance::get_height;
			particle_instance_type["get_width"]        = &graphics::ParticleInstance::get_width;
			particle_instance_type["load"]             = sol::resolve<void(std::string_view)>(&graphics::ParticleInstance::load);
			particle_instance_type["opacity"]          = &graphics::ParticleInstance::opacity;
			particle_instance_type["save"]             = &graphics::ParticleInstance::save;
			particle_instance_type["set_anisotropy"]   = &graphics::ParticleInstance::set_anisotropy;
			particle_instance_type["set_instance"]     = &graphics::ParticleInstance::set_instance;
			particle_instance_type["set_mirrored"]     = &graphics::ParticleInstance::set_mirrored;
			particle_instance_type["set_opacity"]      = &graphics::ParticleInstance::set_opacity;
			particle_instance_type["set_repeated"]     = &graphics::ParticleInstance::set_repeated;
			particle_instance_type["update_instances"] = &graphics::ParticleInstance::update_instances;

			auto particle_generator_type              = lua->new_usertype<graphics::ParticleGenerator>("gParticleGenerator", sol::constructors<graphics::ParticleGenerator()>());
			particle_generator_type["amount"]         = &graphics::ParticleGenerator::amount;
			particle_generator_type["configure"]      = &graphics::ParticleGenerator::configure;
			particle_generator_type["create"]         = &graphics::ParticleGenerator::create;
			particle_generator_type["define"]         = &graphics::ParticleGenerator::define;
			particle_generator_type["finished"]       = &graphics::ParticleGenerator::finished;
			particle_generator_type["gen_circular"]   = &graphics::ParticleGenerator::gen_circular;
			particle_generator_type["gen_linear"]     = &graphics::ParticleGenerator::gen_linear;
			particle_generator_type["update_emitter"] = &graphics::ParticleGenerator::update_emitter;

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

		void register_layerstack()
		{
			auto lua = SL_HANDLE.lua();

			auto layerstack_type     = lua->new_usertype<core::LayerStack>("gLayerStack", sol::no_constructor);
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

		void register_physics()
		{
			auto lua = SL_HANDLE.lua();

			lua->set("GALAXY_PHYSICS_SCALE", physics::SCALE);
			lua->set("GALAXY_FROM_PIXELS_TO_METERS", physics::FROM_PIXELS_TO_METERS);
			lua->set("GALAXY_FROM_METERS_TO_PIXELS", physics::FROM_METERS_TO_PIXELS);

			auto b2vec2_type             = lua->new_usertype<b2Vec2>("b2Vec2", sol::constructors<b2Vec2(), b2Vec2(float, float)>());
			b2vec2_type["Set"]           = &b2Vec2::Set;
			b2vec2_type["SetZero"]       = &b2Vec2::SetZero;
			b2vec2_type["Length"]        = &b2Vec2::Length;
			b2vec2_type["Normalize"]     = &b2Vec2::Normalize;
			b2vec2_type["LengthSquared"] = &b2Vec2::LengthSquared;
			b2vec2_type["Skew"]          = &b2Vec2::Skew;
			b2vec2_type["x"]             = &b2Vec2::x;
			b2vec2_type["y"]             = &b2Vec2::y;

			auto b2_body_type                   = lua->new_usertype<b2Body>("b2Body", sol::no_constructor);
			b2_body_type["ApplyAngularImpulse"] = &b2Body::ApplyAngularImpulse;
			b2_body_type["ApplyForce"]          = &b2Body::ApplyForce;
			b2_body_type["ApplyLinearImpulse"]  = &b2Body::ApplyLinearImpulse;
			b2_body_type["ApplyTorque"]         = &b2Body::ApplyTorque;
			b2_body_type["IsAwake"]             = &b2Body::IsAwake;
			b2_body_type["IsBullet"]            = &b2Body::IsBullet;
			b2_body_type["IsEnabled"]           = &b2Body::IsEnabled;
			b2_body_type["IsFixedRotation"]     = &b2Body::IsFixedRotation;
			b2_body_type["SetAngularDamping"]   = &b2Body::SetAngularDamping;
			b2_body_type["SetAngularVelocity"]  = &b2Body::SetAngularVelocity;
			b2_body_type["SetAwake"]            = &b2Body::SetAwake;
			b2_body_type["SetBullet"]           = &b2Body::SetBullet;
			b2_body_type["SetEnabled"]          = &b2Body::SetEnabled;
			b2_body_type["SetFixedRotation"]    = &b2Body::SetFixedRotation;
			b2_body_type["SetLinearDamping"]    = &b2Body::SetLinearDamping;
			b2_body_type["SetLinearVelocity"]   = &b2Body::SetLinearVelocity;
			b2_body_type["SetSleepingAllowed"]  = &b2Body::SetSleepingAllowed;
			b2_body_type["GetAngle"]            = &b2Body::GetAngle;
			b2_body_type["GetInertia"]          = &b2Body::GetInertia;
			b2_body_type["GetLinearVelocity"]   = &b2Body::GetLinearVelocity;
			b2_body_type["GetMass"]             = &b2Body::GetMass;
			b2_body_type["GetPosition"]         = &b2Body::GetPosition;
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

			auto tex_atlas_type          = lua->new_usertype<res::TextureAtlas>("gTextureAtlas", sol::no_constructor);
			tex_atlas_type["add"]        = &res::TextureAtlas::add;
			tex_atlas_type["create"]     = &res::TextureAtlas::create;
			tex_atlas_type["get_size"]   = &res::TextureAtlas::get_size;
			tex_atlas_type["get_region"] = &res::TextureAtlas::get_region;
			tex_atlas_type["save"]       = &res::TextureAtlas::save;
			tex_atlas_type["update"]     = &res::TextureAtlas::update;

			auto soundbook_type                = lua->new_usertype<res::SoundBook>("gSoundBook", sol::no_constructor);
			soundbook_type["create_from_json"] = &res::SoundBook::create_from_json;
			soundbook_type["get"]              = &res::SoundBook::get;
			soundbook_type["clear"]            = &res::SoundBook::clear;

			auto musicbook_type                = lua->new_usertype<res::MusicBook>("gMusicBook", sol::no_constructor);
			musicbook_type["create_from_json"] = &res::MusicBook::create_from_json;
			musicbook_type["get"]              = &res::MusicBook::get;
			musicbook_type["clear"]            = &res::MusicBook::clear;
		}
	} // namespace lua
} // namespace galaxy