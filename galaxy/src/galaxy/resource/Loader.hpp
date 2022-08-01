///
/// Loader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LOADER_HPP_
#define GALAXY_RESOURCE_LOADER_HPP_

#include <nlohmann/json.hpp>
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
			[[nodiscard]] virtual std::shared_ptr<Resource> create(std::string_view file) = 0;

			///
			/// Load a set of resources recursively from a root folder.
			///
			/// \param folder Folder to recursively search for files.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] virtual Holder<Resource> create_from_folder(std::string_view folder) = 0;

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

			///
			/// Serializes object.
			///
			/// \param holder Reference to the cache holder.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] virtual nlohmann::json internal_serialize(Holder<Resource>& holder) = 0;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] virtual Holder<Resource> internal_deserialize(const nlohmann::json& json) = 0;
		};

		///
		/// Validates template input is derived from a loader.
		///
		/// \tparam T Parent Class to check.
		/// \tparam R Resource Loader is using.
		///
		template<typename T, typename R>
		concept is_loader = std::derived_from<T, Loader<R>>;
	} // namespace resource
} // namespace galaxy

#endif