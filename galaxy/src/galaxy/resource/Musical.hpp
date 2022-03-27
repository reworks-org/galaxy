///
/// Musical.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MUSICAL_HPP_
#define GALAXY_RESOURCE_MUSICAL_HPP_

#include "galaxy/audio/Music.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource loader for music.
		///
		class Musical final : public Loader<audio::Music>
		{
			template<meta::not_memory Resource, is_loader<Resource> Loader>
			friend class Cache;

		public:
			///
			/// Constructor.
			///
			Musical() noexcept;

			///
			/// Destructor.
			///
			virtual ~Musical() noexcept = default;

			///
			/// Load a resource from a file.
			///
			/// \param file Path, including filename, to file in the VFS.
			///
			/// \return Pointer to created resource. Null if creation failed.
			///
			[[nodiscard]] std::shared_ptr<audio::Music> create(std::string_view file) override;

			///
			/// Load a set of resources recursively from a root folder.
			///
			/// \param folder Folder to recursively search for files.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<audio::Music> create_from_folder(std::string_view folder) override;

		private:
			///
			/// Copy constructor.
			///
			Musical(const Musical&) = delete;

			///
			/// Move constructor.
			///
			Musical(Musical&&) = delete;

			///
			/// Copy assignment operator.
			///
			Musical& operator=(const Musical&) = delete;

			///
			/// Move assignment operator.
			///
			Musical& operator=(Musical&&) = delete;

			///
			/// Serializes object.
			///
			/// \param holder Reference to the cache holder.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json internal_serialize(Holder<audio::Music>& holder) override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<audio::Music> internal_deserialize(const nlohmann::json& json) override;
		};
	} // namespace resource
} // namespace galaxy

#endif