///
/// LuaMath.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/math/Base64.hpp"
#include "galaxy/math/FNV1a.hpp"
#include "galaxy/math/Generic.hpp"
#include "galaxy/math/Random.hpp"
#include "galaxy/math/RectPack.hpp"
#include "galaxy/math/ZLib.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_math()
		{
			auto& lua = entt::locator<sol::state>::value();

			lua.set_function("encode_base64", &math::encode_base64);
			lua.set_function("decode_base64", &math::decode_base64);
			lua.set_function("fnv1a_32", &math::fnv1a<std::uint32_t>);
			lua.set_function("fnv1a_64", &math::fnv1a<std::uint64_t>);
			lua.set_function("normalize_f", &math::normalize<float>);
			lua.set_function("normalize_d", &math::normalize<double>);

			auto random_type         = lua.new_usertype<math::Random>("Random", sol::constructors<math::Random()>());
			random_type["gen_int"]   = &math::Random::gen<int>;
			random_type["gen_float"] = &math::Random::gen<float>;
			random_type["gen_vec2f"] = &math::Random::gen_vec2<float>;
			random_type["gen_vec3f"] = &math::Random::gen_vec3<float>;
			random_type["gen_vec2i"] = &math::Random::gen_vec2<int>;
			random_type["gen_vec3i"] = &math::Random::gen_vec3<int>;

			auto rectpack_type              = lua.new_usertype<math::RectPack>("RectPack", sol::constructors<math::RectPack()>());
			rectpack_type["init"]           = &math::RectPack::init;
			rectpack_type["pack"]           = &math::RectPack::pack;
			rectpack_type["clear"]          = &math::RectPack::clear;
			rectpack_type["get_width"]      = &math::RectPack::get_width;
			rectpack_type["get_height"]     = &math::RectPack::get_height;
			rectpack_type["get_free_space"] = &math::RectPack::get_free_space;

			lua.set_function("encode_zlib", &math::encode_zlib);
			lua.set_function("decode_zlib", &math::decode_zlib);
		}
	} // namespace lua
} // namespace galaxy
