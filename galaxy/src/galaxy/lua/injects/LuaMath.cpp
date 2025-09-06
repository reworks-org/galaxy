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
#include "galaxy/math/Math.hpp"
#include "galaxy/math/Random.hpp"
#include "galaxy/math/Rect.hpp"
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

			lua.set_function("fnv1a_32", &math::fnv1a_32);
			lua.set_function("fnv1a_64", &math::fnv1a_64);

			lua.set_function("normalize_f", &math::normalize<float>);
			lua.set_function("normalize_i", &math::normalize<int>);

			lua.set_function("random_int", &math::random<int>);
			lua.set_function("random_float", &math::random<float>);
			lua.set_function("random_vec2", &math::random_vec2);
			lua.set_function("random_vec3", &math::random_vec3);
			lua.set_function("random_vec4", &math::random_vec4);

			lua.set_function("encode_zlib", &ZLib::encode);
			lua.set_function("decode_zlib", &ZLib::decode);

			auto frect_type              = lua.new_usertype<fRect>("fRect", sol::constructors<fRect(), fRect(const float, const float, const float, const float)>());
			frect_type["x"]              = &fRect::m_xpos;
			frect_type["y"]              = &fRect::m_ypos;
			frect_type["width"]          = &fRect::m_width;
			frect_type["height"]         = &fRect::m_height;
			frect_type["contains_point"] = sol::resolve<bool(const float, const float) const>(&fRect::contains);
			frect_type["contains"]       = sol::resolve<bool(const fRect&) const>(&fRect::contains);
			frect_type["overlaps"]       = &fRect::overlaps;
			frect_type["intersects"]     = &fRect::intersects;
			frect_type["set_top_left"]   = &fRect::set_top_left;
			frect_type["get_right"]      = &fRect::get_right;
			frect_type["get_bottom"]     = &fRect::get_bottom;
			frect_type["get_top_left"]   = &fRect::get_top_left;
			frect_type["get_center"]     = &fRect::get_center;
			frect_type["get_size"]       = &fRect::get_size;

			auto irect_type              = lua.new_usertype<iRect>("iRect", sol::constructors<iRect(), iRect(const int, const int, const int, const int)>());
			irect_type["x"]              = &iRect::m_xpos;
			irect_type["y"]              = &iRect::m_ypos;
			irect_type["width"]          = &iRect::m_width;
			irect_type["height"]         = &iRect::m_height;
			irect_type["contains_point"] = sol::resolve<bool(const int, const int) const>(&iRect::contains);
			irect_type["contains"]       = sol::resolve<bool(const iRect&) const>(&iRect::contains);
			irect_type["overlaps"]       = &iRect::overlaps;
			irect_type["intersects"]     = &iRect::intersects;
			irect_type["set_top_left"]   = &iRect::set_top_left;
			irect_type["get_right"]      = &iRect::get_right;
			irect_type["get_bottom"]     = &iRect::get_bottom;
			irect_type["get_top_left"]   = &iRect::get_top_left;
			irect_type["get_center"]     = &iRect::get_center;
			irect_type["get_size"]       = &iRect::get_size;

			auto rectpack_type              = lua.new_usertype<RectPack>("RectPack", sol::constructors<RectPack()>());
			rectpack_type["init"]           = &RectPack::init;
			rectpack_type["pack"]           = &RectPack::pack;
			rectpack_type["clear"]          = &RectPack::clear;
			rectpack_type["get_width"]      = &RectPack::get_width;
			rectpack_type["get_height"]     = &RectPack::get_height;
			rectpack_type["get_free_space"] = &RectPack::get_free_space;
		}
	} // namespace lua
} // namespace galaxy
