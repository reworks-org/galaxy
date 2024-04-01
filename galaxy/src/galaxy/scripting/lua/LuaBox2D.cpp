///
/// LuaBox2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_box2d()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

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
		}
	} // namespace lua
} // namespace galaxy
