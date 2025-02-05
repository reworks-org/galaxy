///
/// LuaMath.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/FNV1a.hpp"
#include "galaxy/math/Generic.hpp"
#include "galaxy/math/Random.hpp"
#include "galaxy/math/Rect.hpp"
#include "galaxy/math/RectPack.hpp"
#include "galaxy/math/ZLib.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_math()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			lua.set_function("encode_base64", &math::encode_base64);
			lua.set_function("decode_base64", &math::decode_base64);
			lua.set_function("fnv1a_32", &math::fnv1a_32);
			lua.set_function("fnv1a_64", &math::fnv1a_64);
			lua.set_function("normalize", &math::normalize<float>);

			auto random_type         = lua.new_usertype<math::Random>("Random", sol::constructors<math::Random()>());
			random_type["gen_int"]   = &math::Random::gen<int>;
			random_type["gen_float"] = &math::Random::gen<float>;
			random_type["gen_vec2"]  = &math::Random::gen_vec2;
			random_type["gen_vec3"]  = &math::Random::gen_vec3;
			random_type["gen_vec4"]  = &math::Random::gen_vec4;

			// clang-format off
			lua.new_enum<math::ZLib::Mode>("ZLibMode",
			{
				{"COMPRESS", math::ZLib::Mode::COMPRESS},
				{"DECOMPRESS", math::ZLib::Mode::DECOMPRESS}
			});
			// clang-format on

			auto frect_type      = lua.new_usertype<math::fRect>("Rect", sol::constructors<math::fRect(), math::fRect(const float, const float, const float, const float)>());
			frect_type["x"]      = &math::fRect::x;
			frect_type["y"]      = &math::fRect::y;
			frect_type["width"]  = &math::fRect::width;
			frect_type["height"] = &math::fRect::height;
			// frect_type["contains_point"] = sol::resolve<bool(const float, const float)>(&math::fRect::contains);
			// frect_type["contains"]       = sol::resolve<bool(const math::fRect&)>(&math::fRect::contains);
			frect_type["overlaps"]     = &math::fRect::overlaps;
			frect_type["intersects"]   = &math::fRect::intersects;
			frect_type["set_top_left"] = &math::fRect::set_top_left;
			frect_type["get_right"]    = &math::fRect::get_right;
			frect_type["get_bottom"]   = &math::fRect::get_bottom;
			frect_type["get_top_left"] = &math::fRect::get_top_left;
			frect_type["get_center"]   = &math::fRect::get_center;
			frect_type["get_size"]     = &math::fRect::get_size;

			auto rectpack_type              = lua.new_usertype<math::RectPack>("RectPack", sol::constructors<math::RectPack()>());
			rectpack_type["clear"]          = &math::RectPack::clear;
			rectpack_type["get_free_space"] = &math::RectPack::get_free_space;
			rectpack_type["get_height"]     = &math::RectPack::get_height;
			rectpack_type["get_width"]      = &math::RectPack::get_width;
			rectpack_type["init"]           = &math::RectPack::init;
			rectpack_type["pack"]           = &math::RectPack::pack;

			// clang-format off
			lua.new_enum<math::ZLib::Mode>("ZLibMode",
			{
				{"COMPRESS", math::ZLib::Mode::COMPRESS},
				{"DECOMPRESS", math::ZLib::Mode::DECOMPRESS}
			});
			// clang-format on

			auto zlibclass_type          = lua.new_usertype<math::ZLib>("ZLib", sol::constructors<math::ZLib(math::ZLib::Mode)>());
			zlibclass_type["compress"]   = &math::ZLib::compress;
			zlibclass_type["decompress"] = &math::ZLib::decompress;
			zlibclass_type["finish"]     = &math::ZLib::finish;

			lua.set_function("encode_zlib", &math::encode_zlib);
			lua.set_function("decode_zlib", &math::decode_zlib);
		}
	} // namespace lua
} // namespace galaxy
