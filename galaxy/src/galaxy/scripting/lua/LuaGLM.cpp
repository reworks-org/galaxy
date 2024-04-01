///
/// LuaGLM.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_glm()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

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
		}
	} // namespace lua
} // namespace galaxy
