///
/// LuaMem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/mem/RingBuffer.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_mem()
		{
			auto& lua = entt::locator<sol::state>::value();

			auto rb_type                   = lua.new_usertype<mem::RingBuffer>("Ringbuffer", sol::constructors<mem::RingBuffer(const int)>());
			rb_type["write"]               = &mem::RingBuffer::write;
			rb_type["read"]                = &mem::RingBuffer::read;
			rb_type["direct_read_pointer"] = &mem::RingBuffer::direct_read_pointer;
			rb_type["available_bytes"]     = &mem::RingBuffer::available_bytes;
			rb_type["set_volume"]          = &mem::RingBuffer::set_volume;
			rb_type["get_volume"]          = &mem::RingBuffer::get_volume;
		}
	} // namespace lua
} // namespace galaxy
