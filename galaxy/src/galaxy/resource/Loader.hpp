///
/// Loader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LOADER_HPP_
#define GALAXY_RESOURCE_LOADER_HPP_

#include <memory>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	///
	/// Loads resources for a resource cache.
	///
	/// \tparam Resource A resource is a class containing data, and cannot be a ref or ptr.
	///
	template<typename Resource>
	requires meta::not_memory<Resource> && meta::is_class<Resource>
	struct Loader
	{
		///
		/// Overloaded operator() used to load a resource.
		///
		/// \tparam Args Argument types.
		///
		/// \param args Forward constructor args.
		///
		/// \return Shared ptr to created resource.
		///
		template<typename... Args>
		inline std::shared_ptr<Resource> operator()(Args&&... args)
		{
			return std::make_shared<Resource>(std::forward<Args>(args)...);
		}
	};
} // namespace galaxy

#endif
