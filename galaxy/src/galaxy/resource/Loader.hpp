///
/// Loader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LOADER_HPP_
#define GALAXY_RESOURCE_LOADER_HPP_

#include <robin_hood.h>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Defines the template syntax for a resource holder.
		///
		template<meta::not_memory Resource>
		using Holder = robin_hood::unordered_flat_map<std::string, std::shared_ptr<Resource>>;

		///
		/// Implements an interface to load a resource from a file or JSON.
		///
		/// \tparam Resource A resource can be anything that is not a pointer or a reference.
		///
		template<meta::not_memory Resource>
		class Loader
		{
		public:
			///
			/// Destructor.
			///
			virtual ~Loader() = default;

			///
			/// Load a resource from a file.
			///
			/// \param file Path, including filename, to file in the VFS.
			///
			/// \return Pointer to created resource. Null if creation failed.
			///
			[[no_discard]] virtual std::shared_ptr<Resource> create(std::string_view file) = 0;

			///
			/// Load a set of resources as defined in a json file.
			///
			/// \param json_file Path, including filename, to file in the VFS.
			///
			/// \return Resource holder containing created resources.
			///
			[[no_discard]] virtual Holder create_from_json(std::string_view json_file) = 0;

		protected:
			///
			/// Constructor.
			///
			Loader() = default;

			///
			/// Copy constructor.
			///
			Loader(const Loader&) = default;

			///
			/// Move constructor.
			///
			Loader(Loader&&) = default;

			///
			/// Copy assignment operator.
			///
			Loader& operator=(const Loader&) = default;

			///
			/// Move assignment operator.
			///
			Loader& operator=(Loader&&) = default;
		};
	} // namespace resource
} // namespace galaxy

#endif