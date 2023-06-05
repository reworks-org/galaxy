///
/// Lua.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entt.hpp>
#include <entt/addons/sol/dispatcher.hpp>
#include <entt/addons/sol/registry.hpp>

#include <FastNoise/FastNoise.h>

#include "galaxy/algorithm/Generic.hpp"
#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/Random.hpp"
#include "galaxy/algorithm/ZLib.hpp"

#include "galaxy/components/Animated.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/LightSource.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/TiledMap.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/core/World.hpp"

#include "galaxy/error/Log.hpp"

#include "galaxy/events/ContentScale.hpp"
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

#include "galaxy/media/AudioEngine.hpp"
#include "galaxy/media/Sound.hpp"

#include "galaxy/platform/Subprocess.hpp"

#include "galaxy/resource/Fonts.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/Materials.hpp"
#include "galaxy/resource/Prefabs.hpp"
#include "galaxy/resource/Scripts.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "galaxy/scene/SceneManager.hpp"

#include "galaxy/scripting/BasicScript.hpp"
#include "galaxy/scripting/LuaExternalLibs.hpp"

#include "galaxy/state/StateMachine.hpp"

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
			auto vec2_type = lua.new_usertype<glm::vec2>(
				"vec2",
				sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
				sol::meta_function::addition,
				[](const glm::vec2& a, const glm::vec2& b) {
					return a + b;
				},
				sol::meta_function::multiplication,
				[](const glm::vec2& a, const glm::vec2& b) {
					return a * b;
				},
				sol::meta_function::subtraction,
				[](const glm::vec2& a, const glm::vec2& b) {
					return a - b;
				},
				sol::meta_function::division,
				[](const glm::vec2& a, const glm::vec2& b) {
					return a / b;
				},
				sol::meta_function::equal_to,
				[](const glm::vec2& a, const glm::vec2& b) {
					return a == b;
				});

			vec2_type["x"] = &glm::vec2::x;
			vec2_type["y"] = &glm::vec2::y;

			auto mult_overloads = sol::overload(
				[](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3 {
					return v1 * v2;
				},
				[](const glm::vec3& v1, float f) -> glm::vec3 {
					return v1 * f;
				},
				[](float f, const glm::vec3& v1) -> glm::vec3 {
					return f * v1;
				});

			auto dvec2_type = lua.new_usertype<glm::dvec2>(
				"dvec2",
				sol::constructors<glm::dvec2(), glm::dvec2(double, double)>(),
				sol::meta_function::addition,
				[](const glm::dvec2& a, const glm::dvec2& b) {
					return a + b;
				},
				sol::meta_function::multiplication,
				[](const glm::dvec2& a, const glm::dvec2& b) {
					return a * b;
				},
				sol::meta_function::subtraction,
				[](const glm::dvec2& a, const glm::dvec2& b) {
					return a - b;
				},
				sol::meta_function::division,
				[](const glm::dvec2& a, const glm::dvec2& b) {
					return a / b;
				},
				sol::meta_function::equal_to,
				[](const glm::dvec2& a, const glm::dvec2& b) {
					return a == b;
				});

			dvec2_type["x"] = &glm::dvec2::x;
			dvec2_type["y"] = &glm::dvec2::y;

			auto vec3_type = lua.new_usertype<glm::vec3>(
				"vec3",
				sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
				sol::meta_function::addition,
				[](const glm::vec3& a, const glm::vec3& b) {
					return a + b;
				},
				sol::meta_function::multiplication,
				mult_overloads,
				sol::meta_function::subtraction,
				[](const glm::vec3& a, const glm::vec3& b) {
					return a - b;
				},
				sol::meta_function::unary_minus,
				[](glm::vec3& v) -> glm::vec3 {
					return -v;
				},
				sol::meta_function::division,
				[](const glm::vec3& a, const glm::vec3& b) {
					return a / b;
				},
				sol::meta_function::equal_to,
				[](const glm::vec3& a, const glm::vec3& b) {
					return a == b;
				});

			vec3_type["x"] = &glm::vec3::x;
			vec3_type["y"] = &glm::vec3::y;
			vec3_type["z"] = &glm::vec3::z;

			auto vec4_type = lua.new_usertype<glm::vec4>(
				"vec4",
				sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
				sol::meta_function::addition,
				[](const glm::vec4& a, const glm::vec4& b) {
					return a + b;
				},
				sol::meta_function::multiplication,
				[](const glm::vec4& a, const glm::vec4& b) {
					return a * b;
				},
				sol::meta_function::multiplication,
				sol::overload(
					[](const glm::vec4& v1, const glm::vec4& v2) -> glm::vec4 {
						return v1 * v2;
					},
					[](const glm::vec4& v1, float f) -> glm::vec4 {
						return v1 * f;
					},
					[](float f, const glm::vec4& v1) -> glm::vec4 {
						return f * v1;
					}),
				sol::meta_function::multiplication,
				[](float a, const glm::vec4& b) {
					return a * b;
				},
				sol::meta_function::subtraction,
				[](const glm::vec4& a, const glm::vec4& b) {
					return a - b;
				},
				sol::meta_function::division,
				[](const glm::vec4& a, const glm::vec4& b) {
					return a / b;
				},
				sol::meta_function::equal_to,
				[](const glm::vec4& a, const glm::vec4& b) {
					return a == b;
				});

			vec4_type["x"] = &glm::vec4::x;
			vec4_type["y"] = &glm::vec4::y;
			vec4_type["z"] = &glm::vec4::z;
			vec4_type["w"] = &glm::vec4::w;

			auto mat3_type = lua.new_usertype<glm::mat3>("mat3",
				sol::constructors<glm::mat3(), glm::mat3(float, float, float, float, float, float, float, float, float)>(),
				sol::meta_function::multiplication,
				[](const glm::mat3& a, const glm::mat3& b) {
					return a * b;
				});

			auto mat4_type = lua.new_usertype<glm::mat4>(
				"mat4",
				sol::constructors<glm::mat4(), glm::mat4(float)>(),
				sol::meta_function::multiplication,
				[](const glm::mat4& a, const glm::mat4& b) {
					return a * b;
				},
				sol::meta_function::addition,
				[](const glm::mat4& a, const glm::mat4& b) {
					return a + b;
				},
				sol::meta_function::subtraction,
				[](const glm::mat4& a, const glm::mat4& b) {
					return a - b;
				});

			mat3_type["length"] = &glm::mat3::length;
			mat4_type["length"] = &glm::mat4::length;

			/* Box2D */
			// clang-format off
			lua.new_enum<b2BodyType>("b2BodyType",
			{
				{"b2_dynamicBody", b2BodyType::b2_dynamicBody},
				{"b2_kinematicBody", b2BodyType::b2_kinematicBody},
				{"b2_staticBody", b2BodyType::b2_staticBody}
			});
			// clang-format on

			lua.new_usertype<b2Vec2>("b2Vec2",
				"x",
				&b2Vec2::x,
				"y",
				&b2Vec2::y,
				"SetZero",
				&b2Vec2::SetZero,
				"Set",
				&b2Vec2::Set,
				"Length",
				&b2Vec2::Length,
				"LengthSquared",
				&b2Vec2::LengthSquared,
				"Normalise",
				&b2Vec2::Normalize,
				"IsValid",
				&b2Vec2::IsValid,
				"Skew",
				&b2Vec2::Skew,
				sol::call_constructor,
				sol::constructors<b2Vec2(), b2Vec2(float, float)>(),
				sol::meta_function::addition,
				sol::overload([](b2Vec2& left, const b2Vec2& right) {
					b2Vec2 ret = left;
					ret += right;
					return ret;
				}),
				sol::meta_function::subtraction,
				sol::overload([](b2Vec2& left, const b2Vec2& right) {
					b2Vec2 ret = left;
					ret -= right;
					return ret;
				}),
				sol::meta_function::multiplication,
				sol::overload(
					[](const float& left, b2Vec2& right) {
						b2Vec2 ret = right;
						ret *= left;
						return ret;
					},
					[](b2Vec2& left, const float& right) {
						b2Vec2 ret = left;
						ret *= right;
						return ret;
					}));

			lua.new_usertype<b2Vec3>("b2Vec3",
				"x",
				&b2Vec3::x,
				"y",
				&b2Vec3::y,
				"z",
				&b2Vec3::z,
				"SetZero",
				&b2Vec3::SetZero,
				"Set",
				&b2Vec3::Set,
				sol::call_constructor,
				sol::constructors<b2Vec3(), b2Vec3(float, float, float)>(),
				sol::meta_function::addition,
				sol::overload([](b2Vec3& left, const b2Vec3& right) {
					b2Vec3 ret = left;
					ret += right;
					return ret;
				}),
				sol::meta_function::subtraction,
				sol::overload([](b2Vec3& left, const b2Vec3& right) {
					b2Vec3 ret = left;
					ret -= right;
					return ret;
				}),
				sol::meta_function::multiplication,
				sol::overload(
					[](const float& left, b2Vec3& right) {
						b2Vec3 ret = right;
						ret *= left;
						return ret;
					},
					[](b2Vec3& left, const float& right) {
						b2Vec3 ret = left;
						ret *= right;
						return ret;
					}));

			auto b2world_type                  = lua.new_usertype<b2World>("b2World", sol::no_constructor);
			b2world_type["ClearForces"]        = &b2World::ClearForces;
			b2world_type["SetAllowSleeping"]   = &b2World::SetAllowSleeping;
			b2world_type["GetAllowSleeping"]   = &b2World::GetAllowSleeping;
			b2world_type["SetWarmStarting"]    = &b2World::SetWarmStarting;
			b2world_type["GetWarmStarting"]    = &b2World::GetWarmStarting;
			b2world_type["SetSubStepping"]     = &b2World::SetSubStepping;
			b2world_type["GetSubStepping"]     = &b2World::GetSubStepping;
			b2world_type["GetProxyCount"]      = &b2World::GetProxyCount;
			b2world_type["GetBodyCount"]       = &b2World::GetBodyCount;
			b2world_type["GetJointCount"]      = &b2World::GetJointCount;
			b2world_type["GetContactCount"]    = &b2World::GetContactCount;
			b2world_type["GetTreeHeight"]      = &b2World::GetTreeHeight;
			b2world_type["GetTreeBalance"]     = &b2World::GetTreeBalance;
			b2world_type["GetTreeQuality"]     = &b2World::GetTreeQuality;
			b2world_type["SetGravity"]         = &b2World::SetGravity;
			b2world_type["GetGravity"]         = &b2World::GetGravity;
			b2world_type["IsLocked"]           = &b2World::IsLocked;
			b2world_type["SetAutoClearForces"] = &b2World::SetAutoClearForces;
			b2world_type["GetAutoClearForces"] = &b2World::GetAutoClearForces;
			b2world_type["ShiftOrigin"]        = &b2World::ShiftOrigin;
			b2world_type["Dump"]               = &b2World::Dump;

			/* ALGORITHM */
			lua.set_function("normalize", &algorithm::normalize<float>);
			lua.set_function("encode_base64", &algorithm::encode_base64);
			lua.set_function("decode_base64", &algorithm::decode_base64);
			lua.set_function("random_int", &algorithm::random<int>);
			lua.set_function("random_float", &algorithm::random<float>);
			lua.set_function("encode_zlib", &algorithm::encode_zlib);
			lua.set_function("decode_zlib", &algorithm::decode_zlib);

			// clang-format off
			lua.new_enum<algorithm::ZLib::Mode>("ZLibMode",
			{
				{"COMPRESS", algorithm::ZLib::Mode::COMPRESS},
				{"DECOMPRESS", algorithm::ZLib::Mode::DECOMPRESS}
			});
			// clang-format on

			auto zlibclass_type          = lua.new_usertype<algorithm::ZLib>("ZLib", sol::constructors<algorithm::ZLib(algorithm::ZLib::Mode)>());
			zlibclass_type["compress"]   = &algorithm::ZLib::compress;
			zlibclass_type["decompress"] = &algorithm::ZLib::decompress;
			zlibclass_type["finish"]     = &algorithm::ZLib::finish;

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

			auto flag_type =
				lua.new_usertype<components::Flag>("Flag", sol::constructors<components::Flag()>(), "type_id", &entt::type_hash<components::Flag>::value);
			flag_type["set_enabled"]            = &components::Flag::set_flag<flags::Enabled>;
			flag_type["unset_enabled"]          = &components::Flag::unset_flag<flags::Enabled>;
			flag_type["is_enabled_set"]         = &components::Flag::is_flag_set<flags::Enabled>;
			flag_type["set_allow_serialize"]    = &components::Flag::set_flag<flags::AllowSerialize>;
			flag_type["unset_allow_serialize"]  = &components::Flag::unset_flag<flags::AllowSerialize>;
			flag_type["is_allow_serialize_set"] = &components::Flag::is_flag_set<flags::AllowSerialize>;

			entt_sol::register_meta_component<components::Flag>();

			auto lightdata_type        = lua.new_usertype<graphics::Light>("Light", sol::constructors<graphics::Light()>());
			lightdata_type["colour"]   = &graphics::Light::colour;
			lightdata_type["pos"]      = &graphics::Light::pos;
			lightdata_type["falloff"]  = &graphics::Light::falloff;
			lightdata_type["depth"]    = &graphics::Light::depth;
			lightdata_type["diameter"] = &graphics::Light::diameter;

			auto light_type     = lua.new_usertype<components::LightSource>("Light",
                sol::constructors<components::LightSource()>(),
                "type_id",
                &entt::type_hash<components::LightSource>::value);
			light_type["light"] = &components::LightSource::m_light;

			entt_sol::register_meta_component<components::LightSource>();

			auto mapcomponent_type =
				lua.new_usertype<components::Map>("Map", sol::constructors<components::Map()>(), "type_id", &entt::type_hash<components::Map>::value);

			entt_sol::register_meta_component<components::Map>();

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

			entt_sol::register_meta_component<components::RigidBody>();

			auto script_type = lua.new_usertype<components::Script>("Script",
				sol::constructors<components::Script()>(),
				"type_id",
				&entt::type_hash<components::Script>::value);

			script_type["file"]   = &components::Script::file;
			script_type["self"]   = &components::Script::m_self;
			script_type["update"] = &components::Script::m_update;

			entt_sol::register_meta_component<components::Script>();

			auto sprite_type = lua.new_usertype<components::Sprite>("Sprite",
				sol::constructors<components::Sprite()>(),
				"type_id",
				&entt::type_hash<components::Sprite>::value);

			sprite_type["create"] = sol::resolve<void(const std::string&, const int, const float, const bool)>(&components::Sprite::create);
			sprite_type["create_texturerect"] =
				sol::resolve<void(const std::string&, const graphics::iRect&, const int, const float, const bool)>(&components::Sprite::create);
			sprite_type["update"]             = sol::resolve<void(const std::string&)>(&components::Sprite::update);
			sprite_type["update_texturerect"] = sol::resolve<void(const std::string&, const graphics::iRect&)>(&components::Sprite::update);
			sprite_type["get_height"]         = &components::Sprite::get_height;
			sprite_type["get_opacity"]        = &components::Sprite::get_opacity;
			sprite_type["get_width"]          = &components::Sprite::get_width;
			sprite_type["set_opacity"]        = &components::Sprite::set_opacity;
			sprite_type["is_normal_mapped"]   = &components::Sprite::is_normal_mapped;

			entt_sol::register_meta_component<components::Sprite>();

			auto tag_type =
				lua.new_usertype<components::Tag>("Tag", sol::constructors<components::Tag()>(), "type_id", &entt::type_hash<components::Tag>::value);
			tag_type["tag"] = &components::Tag::m_tag;

			entt_sol::register_meta_component<components::Tag>();

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

			// Cannot be created in lua, accessed from scene instead.
			auto world_type                   = lua.new_usertype<core::World>("World", sol::no_constructor);
			world_type["clear"]               = &core::World::clear;
			world_type["registry"]            = &core::World::m_registry;
			world_type["create"]              = &core::World::create;
			world_type["create_from_prefab"]  = &core::World::create_from_prefab;
			world_type["is_valid"]            = &core::World::is_valid;
			world_type["pixels_per_meter"]    = &core::World::m_pixels_per_meter;
			world_type["velocity_iterations"] = &core::World::m_velocity_iterations;
			world_type["position_iterations"] = &core::World::m_position_iterations;
			world_type["b2world"]             = &core::World::b2world;

			auto tiledmap_type           = lua.new_usertype<core::TiledMap>("TiledMap", sol::no_constructor);
			tiledmap_type["disable_map"] = &core::TiledMap::disable_map;
			tiledmap_type["enable_map"]  = &core::TiledMap::enable_map;
			tiledmap_type["get_name"]    = &core::TiledMap::get_name;
			tiledmap_type["load_map"]    = &core::TiledMap::load_map;

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

			entt_sol::register_meta_event<events::ContentScale>();

			auto keychar_type         = lua.new_usertype<events::KeyChar>("KeyChar", sol::constructors<events::KeyChar()>());
			keychar_type["type_id"]   = &entt::type_hash<events::KeyChar>::value;
			keychar_type["codepoint"] = &events::KeyChar::codepoint;
			keychar_type["handled"]   = &events::KeyChar::handled;

			entt_sol::register_meta_event<events::KeyChar>();

			auto keydown_type        = lua.new_usertype<events::KeyDown>("KeyDown", sol::constructors<events::KeyDown()>());
			keydown_type["type_id"]  = &entt::type_hash<events::KeyDown>::value;
			keydown_type["keycode"]  = &events::KeyDown::keycode;
			keydown_type["mod"]      = &events::KeyDown::mod;
			keydown_type["scancode"] = &events::KeyDown::scancode;
			keydown_type["handled"]  = &events::KeyDown::handled;

			entt_sol::register_meta_event<events::KeyDown>();

			auto keyup_type        = lua.new_usertype<events::KeyUp>("KeyUp", sol::constructors<events::KeyUp()>());
			keyup_type["type_id"]  = &entt::type_hash<events::KeyUp>::value;
			keyup_type["keycode"]  = &events::KeyUp::keycode;
			keyup_type["mod"]      = &events::KeyUp::mod;
			keyup_type["scancode"] = &events::KeyUp::scancode;
			keyup_type["handled"]  = &events::KeyUp::handled;

			entt_sol::register_meta_event<events::KeyUp>();

			auto mousemoved_type       = lua.new_usertype<events::MouseMoved>("MouseMoved", sol::constructors<events::MouseMoved()>());
			mousemoved_type["type_id"] = &entt::type_hash<events::MouseMoved>::value;
			mousemoved_type["xpos"]    = &events::MouseMoved::xpos;
			mousemoved_type["ypos"]    = &events::MouseMoved::ypos;
			mousemoved_type["handled"] = &events::MouseMoved::handled;

			entt_sol::register_meta_event<events::MouseMoved>();

			auto mousepressed_type       = lua.new_usertype<events::MousePressed>("MousePressed", sol::constructors<events::MousePressed()>());
			mousepressed_type["type_id"] = &entt::type_hash<events::MousePressed>::value;
			mousepressed_type["button"]  = &events::MousePressed::button;
			mousepressed_type["mod"]     = &events::MousePressed::mod;
			mousepressed_type["xpos"]    = &events::MousePressed::xpos;
			mousepressed_type["ypos"]    = &events::MousePressed::ypos;
			mousepressed_type["handled"] = &events::MousePressed::handled;

			entt_sol::register_meta_event<events::MousePressed>();

			auto mousereleased_type       = lua.new_usertype<events::MouseReleased>("MouseReleased", sol::constructors<events::MouseReleased()>());
			mousereleased_type["type_id"] = &entt::type_hash<events::MouseReleased>::value;
			mousereleased_type["button"]  = &events::MouseReleased::button;
			mousereleased_type["mod"]     = &events::MouseReleased::mod;
			mousereleased_type["xpos"]    = &events::MouseReleased::xpos;
			mousereleased_type["ypos"]    = &events::MouseReleased::ypos;
			mousereleased_type["handled"] = &events::MouseReleased::handled;

			entt_sol::register_meta_event<events::MouseReleased>();

			auto mousewheel_type       = lua.new_usertype<events::MouseWheel>("MouseWheel", sol::constructors<events::MouseWheel()>());
			mousewheel_type["type_id"] = &entt::type_hash<events::MouseWheel>::value;
			mousewheel_type["xoff"]    = &events::MouseWheel::xoff;
			mousewheel_type["yoff"]    = &events::MouseWheel::yoff;
			mousewheel_type["handled"] = &events::MouseWheel::handled;

			entt_sol::register_meta_event<events::MouseWheel>();

			auto windowresized_type       = lua.new_usertype<events::WindowResized>("WindowResized", sol::constructors<events::WindowResized()>());
			windowresized_type["type_id"] = &entt::type_hash<events::WindowResized>::value;
			windowresized_type["width"]   = &events::WindowResized::width;
			windowresized_type["height"]  = &events::WindowResized::height;

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

			lua.new_enum<fs::DialogIcon>("DialogIcons",
			{
                {"info", fs::DialogIcon::info},
				{"warning", fs::DialogIcon::warning},
				{"error", fs::DialogIcon::error}
            });
			// clang-format on

			auto fileinfo_type      = lua.new_usertype<fs::FileInfo>("FileInfo", sol::constructors<fs::FileInfo()>());
			fileinfo_type["code"]   = &fs::FileInfo::code;
			fileinfo_type["string"] = &fs::FileInfo::string;

			auto vfs_type                        = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["alert"]                    = &fs::VirtualFileSystem::alert;
			vfs_type["create_file"]              = &fs::VirtualFileSystem::create_file;
			vfs_type["create_folder"]            = &fs::VirtualFileSystem::create_folder;
			vfs_type["exists"]                   = &fs::VirtualFileSystem::exists;
			vfs_type["find"]                     = &fs::VirtualFileSystem::find;
			vfs_type["list_directory"]           = &fs::VirtualFileSystem::list_directory;
			vfs_type["open"]                     = &fs::VirtualFileSystem::open;
			vfs_type["open_binary"]              = &fs::VirtualFileSystem::open_binary;
			vfs_type["open_binary_using_dialog"] = &fs::VirtualFileSystem::open_binary_using_dialog;
			vfs_type["open_file_dialog"]         = &fs::VirtualFileSystem::open_file_dialog;
			vfs_type["open_file_dialog_multi"]   = &fs::VirtualFileSystem::open_file_dialog_multi;
			vfs_type["open_save_dialog"]         = &fs::VirtualFileSystem::open_save_dialog;
			vfs_type["open_using_dialog"]        = &fs::VirtualFileSystem::open_using_dialog;
			vfs_type["remove"]                   = &fs::VirtualFileSystem::remove;
			vfs_type["root"]                     = &fs::VirtualFileSystem::root;
			vfs_type["save"]                     = &fs::VirtualFileSystem::save;
			vfs_type["save_binary"]              = &fs::VirtualFileSystem::save_binary;
			vfs_type["save_binary_using_dialog"] = &fs::VirtualFileSystem::save_binary_using_dialog;
			vfs_type["save_using_dialog"]        = &fs::VirtualFileSystem::save_using_dialog;
			vfs_type["select_folder_dialog"]     = &fs::VirtualFileSystem::select_folder_dialog;
			vfs_type["trigger_notification"]     = &fs::VirtualFileSystem::trigger_notification;

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

			auto colour_type        = lua.new_usertype<graphics::Colour>("Colour",
                sol::constructors<graphics::Colour(), graphics::Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
			colour_type["alpha"]    = &graphics::Colour::m_alpha;
			colour_type["blue"]     = &graphics::Colour::m_blue;
			colour_type["green"]    = &graphics::Colour::m_green;
			colour_type["red"]      = &graphics::Colour::m_red;
			colour_type["to_array"] = &graphics::Colour::to_array;
			colour_type["to_vec4"]  = &graphics::Colour::to_vec4;

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
			uniform_type["count"]    = &graphics::UniformInfo::count;
			uniform_type["location"] = &graphics::UniformInfo::location;

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

			/* MEDIA */
			// clang-format off
			lua.new_enum<media::Sound::Type>("SoundType",
			{
				{"DIALOGUE", media::Sound::Type::DIALOGUE},
				{"MUSIC", media::Sound::Type::MUSIC},
				{"SFX", media::Sound::Type::SFX}
			});
			
			lua.new_enum<ma_positioning>("ma_positioning",
			{
				{"ma_positioning_absolute", ma_positioning::ma_positioning_absolute},
				{"ma_positioning_relative", ma_positioning::ma_positioning_relative}
			});
			
			lua.new_enum<ma_attenuation_model>("ma_attenuation_model",
			{
				{"ma_attenuation_model_exponential", ma_attenuation_model::ma_attenuation_model_exponential},
				{"ma_attenuation_model_inverse", ma_attenuation_model::ma_attenuation_model_inverse},
				{"ma_attenuation_model_linear", ma_attenuation_model::ma_attenuation_model_linear},
				{"ma_attenuation_model_none", ma_attenuation_model::ma_attenuation_model_none}
			});
			// clang-format on

			auto sound_type = lua.new_usertype<media::Sound>("Sound", sol::constructors<media::Sound(), media::Sound(media::Sound::Type, std::string_view)>());
			sound_type["fade_in"]               = &media::Sound::fade_in;
			sound_type["fade_out"]              = &media::Sound::fade_out;
			sound_type["is_finished"]           = &media::Sound::is_finished;
			sound_type["is_looping"]            = &media::Sound::is_looping;
			sound_type["is_playing"]            = &media::Sound::is_playing;
			sound_type["load"]                  = &media::Sound::load;
			sound_type["set_attenuation_model"] = &media::Sound::set_attenuation_model;
			sound_type["set_cone"]              = &media::Sound::set_cone;
			sound_type["set_direction"]         = &media::Sound::set_direction;
			sound_type["set_doppler_factor"]    = &media::Sound::set_doppler_factor;
			sound_type["set_looping"]           = &media::Sound::set_looping;
			sound_type["set_max_distance"]      = &media::Sound::set_max_distance;
			sound_type["set_max_gain"]          = &media::Sound::set_max_gain;
			sound_type["set_min_distance"]      = &media::Sound::set_min_distance;
			sound_type["set_min_gain"]          = &media::Sound::set_min_gain;
			sound_type["set_pan"]               = &media::Sound::set_pan;
			sound_type["set_pinned_listener"]   = &media::Sound::set_pinned_listener;
			sound_type["set_pitch"]             = &media::Sound::set_pitch;
			sound_type["set_position"]          = &media::Sound::set_position;
			sound_type["set_positioning"]       = &media::Sound::set_positioning;
			sound_type["set_rolloff"]           = &media::Sound::set_rolloff;
			sound_type["set_velocity"]          = &media::Sound::set_velocity;
			sound_type["set_volume"]            = &media::Sound::set_volume;
			sound_type["play"]                  = &media::Sound::play;
			sound_type["pause"]                 = &media::Sound::pause;
			sound_type["stop"]                  = &media::Sound::stop;
			sound_type["toggle_spatialization"] = &media::Sound::toggle_spatialization;

			auto audioengine_type                      = lua.new_usertype<media::AudioEngine>("AudioEngine", sol::no_constructor);
			audioengine_type["set_sfx_volume"]         = &media::AudioEngine::set_sfx_volume;
			audioengine_type["set_music_volume"]       = &media::AudioEngine::set_music_volume;
			audioengine_type["set_dialogue_volume"]    = &media::AudioEngine::set_dialogue_volume;
			audioengine_type["set_listener_cone"]      = &media::AudioEngine::set_listener_cone;
			audioengine_type["set_listener_direction"] = &media::AudioEngine::set_listener_direction;
			audioengine_type["set_listener_position"]  = &media::AudioEngine::set_listener_position;
			audioengine_type["set_listener_world_up"]  = &media::AudioEngine::set_listener_world_up;
			audioengine_type["toggle_listener"]        = &media::AudioEngine::toggle_listener;
			audioengine_type["stop"]                   = &media::AudioEngine::stop;

			/* PHYSICS */
			auto material_type                     = lua.new_usertype<physics::Material>("Material", sol::constructors<physics::Material()>());
			material_type["density"]               = &physics::Material::density;
			material_type["friction"]              = &physics::Material::friction;
			material_type["restitution"]           = &physics::Material::restitution;
			material_type["restitution_threshold"] = &physics::Material::restitution_threshold;

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

			auto materials_type     = lua.new_usertype<resource::Materials>("Materials", sol::no_constructor);
			materials_type["clear"] = &resource::Materials::clear;
			materials_type["empty"] = &resource::Materials::empty;
			materials_type["get"]   = &resource::Materials::get;
			materials_type["has"]   = &resource::Materials::has;
			materials_type["load"]  = &resource::Materials::load;
			materials_type["keys"]  = &resource::Materials::keys;

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
			auto scenemanager_type            = lua.new_usertype<scene::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["all"]          = &scene::SceneManager::all;
			scenemanager_type["clear"]        = &scene::SceneManager::clear;
			scenemanager_type["current"]      = &scene::SceneManager::current;
			scenemanager_type["get"]          = &scene::SceneManager::get;
			scenemanager_type["has_current"]  = &scene::SceneManager::has_current;
			scenemanager_type["load_appdata"] = &scene::SceneManager::load_appdata;
			scenemanager_type["load_assets"]  = &scene::SceneManager::load_assets;
			scenemanager_type["load_scene"]   = &scene::SceneManager::load_scene;
			scenemanager_type["make_scene"]   = &scene::SceneManager::make_scene;
			scenemanager_type["remove"]       = &scene::SceneManager::remove;
			scenemanager_type["save_appdata"] = &scene::SceneManager::save_appdata;
			scenemanager_type["set_scene"]    = &scene::SceneManager::set_scene;
			scenemanager_type["unload_scene"] = &scene::SceneManager::unload_scene;

			// Use scenemanager to create.
			auto scene_type          = lua.new_usertype<scene::Scene>("Scene", sol::no_constructor);
			scene_type["camera"]     = &scene::Scene::m_camera;
			scene_type["context"]    = &scene::Scene::m_context;
			scene_type["dispatcher"] = &scene::Scene::m_dispatcher;
			scene_type["map"]        = &scene::Scene::m_map;
			scene_type["name"]       = &scene::Scene::m_name;
			scene_type["world"]      = &scene::Scene::m_world;

			/* SCRIPTING */
			auto basicscript_type =
				lua.new_usertype<lua::BasicScript>("BasicScript", sol::constructors<lua::BasicScript(), lua::BasicScript(std::string_view)>());
			basicscript_type["load"]           = &lua::BasicScript::load;
			basicscript_type["run"]            = &lua::BasicScript::run;
			basicscript_type["run_and_return"] = &lua::BasicScript::run_and_return;

			/* STATE */
			auto state_type             = lua.new_usertype<state::State>("State", sol::no_constructor);
			auto statemachine_type      = lua.new_usertype<state::StateMachine>("StateMachine", sol::constructors<state::StateMachine()>());
			statemachine_type["pop"]    = &state::StateMachine::pop;
			statemachine_type["push"]   = &state::StateMachine::push;
			statemachine_type["update"] = &state::StateMachine::update;

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
			lua.set("GALAXY_MIN_CAMERA_ZOOM", GALAXY_MIN_CAMERA_ZOOM);
			lua.set("GALAXY_MAX_CAMERA_ZOOM", GALAXY_MAX_CAMERA_ZOOM);
			lua.set("GALAXY_DEFAULT_RENDERER_RESERVED", GALAXY_DEFAULT_RENDERER_RESERVED);
			lua.set("GALAXY_FONT_MSDF_RANGE", GALAXY_FONT_MSDF_RANGE);
			lua.set("GALAXY_FONT_MSDF_SCALE", GALAXY_FONT_MSDF_SCALE);
			lua.set("GALAXY_DEFAULT_ELLIPSE_FRAGMENTS", GALAXY_DEFAULT_ELLIPSE_FRAGMENTS);
			lua.set("GALAXY_APP_DIR", GALAXY_APP_DIR);
			lua.set("GALAXY_WORKER_THREADS", GALAXY_WORKER_THREADS);
			lua.set("GALAXY_ZLIB_COMPLETE_CHUNK", GALAXY_ZLIB_COMPLETE_CHUNK);
			lua.set("GALAXY_EXIT_SUCCESS", GALAXY_EXIT_SUCCESS);
			lua.set("GALAXY_EXIT_FAILURE", GALAXY_EXIT_FAILURE);

			lua.set_function("galaxy_str_begins_with", &strutils::begins_with);
		}

		void inject_services_into_lua()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua["galaxy_config"]        = std::ref(core::ServiceLocator<core::Config>::ref());
			lua["galaxy_language"]      = std::ref(core::ServiceLocator<resource::Language>::ref());
			lua["galaxy_maps"]          = std::ref(core::ServiceLocator<resource::Maps>::ref());
			lua["galaxy_shaders"]       = std::ref(core::ServiceLocator<resource::Shaders>::ref());
			lua["galaxy_fonts"]         = std::ref(core::ServiceLocator<resource::Fonts>::ref());
			lua["galaxy_sounds"]        = std::ref(core::ServiceLocator<resource::Sounds>::ref());
			lua["galaxy_textureatlas"]  = std::ref(core::ServiceLocator<resource::TextureAtlas>::ref());
			lua["galaxy_prefabs"]       = std::ref(core::ServiceLocator<resource::Prefabs>::ref());
			lua["galaxy_materials"]     = std::ref(core::ServiceLocator<resource::Materials>::ref());
			lua["galaxy_audioengine"]   = std::ref(core::ServiceLocator<media::AudioEngine>::ref());
			lua["galaxy_scripts"]       = std::ref(core::ServiceLocator<resource::Scripts>::ref());
			lua["galaxy_state_manager"] = std::ref(core::ServiceLocator<scene::SceneManager>::ref());
			lua["service_fs"]           = std::ref(core::ServiceLocator<fs::VirtualFileSystem>::ref());
		}

		void load_external_libs()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.script(scripting::inspect);
			lua.script(scripting::middleclass);
			lua.script(scripting::serpent);
		}
	} // namespace lua
} // namespace galaxy