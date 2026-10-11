///
/// LuaGraphics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/graphics/Shader.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_graphics() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto shader_type         = lua.new_usertype<Shader>("Shader", sol::constructors<Shader(), Shader(const std::string&), Shader(const std::string&, const std::string&)>());
		shader_type["load"]      = sol::resolve<bool(const std::string&)>(&Shader::load);
		shader_type["load_src"]  = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::load);
		shader_type["parse"]     = sol::resolve<bool(const std::string&)>(&Shader::parse);
		shader_type["parse_src"] = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::parse);
		shader_type["set_uniform_tex2d"]  = &Shader::set_uniform<ray::Texture2D>;
		shader_type["set_uniform_matrix"] = &Shader::set_uniform<ray::Matrix>;
		shader_type["set_uniform_vec4"]   = &Shader::set_uniform<ray::Vector4>;
		shader_type["set_uniform_vec3"]   = &Shader::set_uniform<ray::Vector3>;
		shader_type["set_uniform_vec2"]   = &Shader::set_uniform<ray::Vector2>;
		shader_type["set_uniform_float"]  = &Shader::set_uniform<float>;
		shader_type["set_uniform_uint"]   = &Shader::set_uniform<unsigned int>;
		shader_type["set_uniform_int"]    = &Shader::set_uniform<int>;
		shader_type["set_uniform_bool"]   = &Shader::set_uniform<bool>;
	}
} // namespace galaxy
