///
/// Scripts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SCRIPTS_HPP_
#define GALAXY_RESOURCE_SCRIPTS_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"
#include "galaxy/scripting/Script.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for lua scripts cache.
		///
		using Scripts = Cache<lua::Script, Loader<lua::Script>>;
	} // namespace resource
} // namespace galaxy

#endif
