///
/// LuaDependencies.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <entt_sol/dispatcher.hpp>
#include <entt_sol/registry.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <miniaudio.h>

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
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/events/ContentScale.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/MouseEnter.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_dependencies()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			/* ENTT */
			lua.require("registry", sol::c_call<decltype(&entt_sol::open_registry), &entt_sol::open_registry>, false);
			lua.require("dispatcher", sol::c_call<decltype(&entt_sol::open_dispatcher), &entt_sol::open_dispatcher>, false);

			entt_sol::register_meta_event<events::WindowResized>();
			entt_sol::register_meta_event<events::WindowClosed>();
			entt_sol::register_meta_event<events::MouseWheel>();
			entt_sol::register_meta_event<events::MouseReleased>();
			entt_sol::register_meta_event<events::MousePressed>();
			entt_sol::register_meta_event<events::MouseMoved>();
			entt_sol::register_meta_event<events::MouseEnter>();
			entt_sol::register_meta_event<events::KeyUp>();
			entt_sol::register_meta_event<events::KeyRepeat>();
			entt_sol::register_meta_event<events::KeyDown>();
			entt_sol::register_meta_event<events::KeyChar>();
			entt_sol::register_meta_event<events::ContentScale>();
			entt_sol::register_meta_component<components::Transform>();
			entt_sol::register_meta_component<components::Text>();
			entt_sol::register_meta_component<components::Tag>();
			entt_sol::register_meta_component<components::Sprite>();
			entt_sol::register_meta_component<components::Script>();
			entt_sol::register_meta_component<components::RigidBody>();
			entt_sol::register_meta_component<components::Primitive>();
			entt_sol::register_meta_component<components::Animated>();
			entt_sol::register_meta_component<components::MapData>();
			entt_sol::register_meta_component<components::ParticleGenerator>();
			entt_sol::register_meta_component<components::UIScript>();
			entt_sol::register_meta_component<flags::DenySerialization>();
			entt_sol::register_meta_component<flags::Disabled>();

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
					b2Vec2 ret  = left;
					ret        += right;
					return ret;
				}),
				sol::meta_function::subtraction,
				sol::overload([](b2Vec2& left, const b2Vec2& right) {
					b2Vec2 ret  = left;
					ret        -= right;
					return ret;
				}),
				sol::meta_function::multiplication,
				sol::overload(
					[](const float& left, b2Vec2& right) {
						b2Vec2 ret  = right;
						ret        *= left;
						return ret;
					},
					[](b2Vec2& left, const float& right) {
						b2Vec2 ret  = left;
						ret        *= right;
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
					b2Vec3 ret  = left;
					ret        += right;
					return ret;
				}),
				sol::meta_function::subtraction,
				sol::overload([](b2Vec3& left, const b2Vec3& right) {
					b2Vec3 ret  = left;
					ret        -= right;
					return ret;
				}),
				sol::meta_function::multiplication,
				sol::overload(
					[](const float& left, b2Vec3& right) {
						b2Vec3 ret  = right;
						ret        *= left;
						return ret;
					},
					[](b2Vec3& left, const float& right) {
						b2Vec3 ret  = left;
						ret        *= right;
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

			// clang-format off
			lua.new_enum<ma_pan_mode>("ma_pan_mode",
			{
				{"ma_pan_mode_balance", ma_pan_mode_balance},
				{"ma_pan_mode_pan", ma_pan_mode_pan}
			});

			lua.new_enum<ma_positioning>("ma_positioning",
            {
				{"ma_positioning_absolute", ma_positioning_absolute},
				{"ma_positioning_relative", ma_positioning_relative}
            });

			lua.new_enum<ma_attenuation_model>("ma_attenuation_model",
            {
				{"ma_attenuation_model_none", ma_attenuation_model_none},
				{"ma_attenuation_model_inverse", ma_attenuation_model_inverse},
				{"ma_attenuation_model_linear", ma_attenuation_model_linear},
				{"ma_attenuation_model_exponential", ma_attenuation_model_exponential}
            });
			// clang-format on

			auto ma_vec3f_type = lua.new_usertype<ma_vec3f>(sol::constructors<ma_vec3f()>());
			ma_vec3f_type["x"] = &ma_vec3f::x;
			ma_vec3f_type["y"] = &ma_vec3f::y;
			ma_vec3f_type["z"] = &ma_vec3f::z;
		}
	} // namespace lua
} // namespace galaxy
