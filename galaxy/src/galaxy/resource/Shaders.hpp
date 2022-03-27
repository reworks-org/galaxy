///
/// Shaders.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SHADERS_HPP_
#define GALAXY_RESOURCE_SHADERS_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// \brief Resource loader for Shaders.
		///
		/// Shaders are required to have an extension of .vs for vertex shaders
		/// and .fs for fragment shaders.
		/// Or whatever has been configured in GALAXY GLOBALS CONFIG.
		///
		class Shaders final : public Loader<graphics::Shader>
		{
			template<meta::not_memory Resource, is_loader<Resource> Loader>
			friend class Cache;

		public:
			///
			/// Constructor.
			///
			Shaders() noexcept;

			///
			/// Destructor.
			///
			virtual ~Shaders() noexcept = default;

			///
			/// \brief Load a resource from a file.
			///
			/// Paired shaders must share a common filename.
			///
			/// \param file Path, including filename, to file in the VFS.
			///
			/// \return Pointer to created resource. Null if creation failed.
			///
			[[nodiscard]] std::shared_ptr<graphics::Shader> create(std::string_view file) override;

			///
			/// Load a set of resources recursively from a root folder.
			///
			/// \param folder Folder to recursively search for files.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<graphics::Shader> create_from_folder(std::string_view folder) override;

		private:
			///
			/// Copy constructor.
			///
			Shaders(const Shaders&) = delete;

			///
			/// Move constructor.
			///
			Shaders(Shaders&&) = delete;

			///
			/// Copy assignment operator.
			///
			Shaders& operator=(const Shaders&) = delete;

			///
			/// Move assignment operator.
			///
			Shaders& operator=(Shaders&&) = delete;

			///
			/// Serializes object.
			///
			/// \param holder Reference to the cache holder.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json internal_serialize(Holder<graphics::Shader>& holder) override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			/// \return Resource holder containing loaded resources.
			///
			[[nodiscard]] Holder<graphics::Shader> internal_deserialize(const nlohmann::json& json) override;
		};
	} // namespace resource
} // namespace galaxy

#endif