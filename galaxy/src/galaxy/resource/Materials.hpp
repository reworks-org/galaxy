///
/// Materials.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MATERIALS_HPP_
#define GALAXY_RESOURCE_MATERIALS_HPP_

#include "galaxy/physics/Material.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for a physics material.
		///
		class MaterialLoader final
		{
		public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<physics::Material> operator()(const std::string& file);
		};

		///
		/// Abbreviation for materials cache type.
		///
		using Materials = Cache<physics::Material, MaterialLoader, false>;
	} // namespace resource
} // namespace galaxy

#endif