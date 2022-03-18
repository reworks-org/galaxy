///
/// Sounds.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SOUNDS_HPP_
#define GALAXY_RESOURCE_SOUNDS_HPP_

#include "galaxy/audio/Sound.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource loader for sounds.
		///
		class Sounds final : public Loader<audio::Sound>
		{
			template<meta::not_memory Resource, is_loader<Resource> Loader>
			friend class Cache;

		public:
			///
			/// Constructor.
			///
			Sounds() noexcept;

			///
			/// Argument Constructor.
			///
			/// \param folder Folder to recursively search for files.
			///
			Sounds(std::string_view folder);

			///
			/// Deserialization Constructor.
			///
			/// \param json Json object to retrieve data from.
			///
			Sounds(const nlohmann::json& json);

			///
			/// Destructor.
			///
			virtual ~Sounds() noexcept = default;

			///
			/// Load a resource from a file.
			///
			/// \param file Path, including filename, to file in the VFS.
			///
			/// \return Pointer to created resource. Null if creation failed.
			///
			[[nodiscard]] std::shared_ptr<audio::Sound> create(std::string_view file) override;

			///
			/// Load a set of resources recursively from a root folder.
			///
			/// \param folder Folder to recursively search for files.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<audio::Sound> create_from_folder(std::string_view folder) override;

		private:
			///
			/// Copy constructor.
			///
			Sounds(const Sounds&) = delete;

			///
			/// Move constructor.
			///
			Sounds(Sounds&&) = delete;

			///
			/// Copy assignment operator.
			///
			Sounds& operator=(const Sounds&) = delete;

			///
			/// Move assignment operator.
			///
			Sounds& operator=(Sounds&&) = delete;

			///
			/// Serializes object.
			///
			/// \param holder Reference to the cache holder.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json internal_serialize(Holder<audio::Sound>& holder) override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<audio::Sound> internal_deserialize(const nlohmann::json& json) override;
		};
	} // namespace resource
} // namespace galaxy

#endif