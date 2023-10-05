///
/// BasicScripts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_BASICSCRIPTS_HPP_
#define GALAXY_RESOURCE_BASICSCRIPTS_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/scripting/BasicScript.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for lua scripts.
		///
		class BasicScriptLoader final
		{
		public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<lua::BasicScript> operator()(const std::string& file);
		};

		///
		/// Abbreviation for basic lua scripts cache.
		///
		using BasicScripts = Cache<lua::BasicScript, BasicScriptLoader, false>;
	} // namespace resource
} // namespace galaxy

#endif