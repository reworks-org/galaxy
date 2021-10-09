///
/// Model.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COMPONENTS_MODEL_HPP_
#define GALAXY_GRAPHICS_COMPONENTS_MODEL_HPP_

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Mesh.hpp"
#include "galaxy/res/MaterialBook.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 3D Model (Collection of meshes + materials).
		///
		class Model final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Model() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Model(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Model(Model&&) noexcept;

			///
			/// Move assignment operator.
			///
			Model& operator=(Model&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Model() noexcept;

			///
			/// Load model from disk.
			///
			/// \param file File on disk in VFS.
			///
			/// \return True if load was sucessful.
			///
			[[nodiscard]] const bool load(std::string_view file);

			///
			/// Create mesh VAOs.
			///
			void create();

			///
			/// Retrieve meshes.
			///
			/// \return Reference to std::vector of meshes.
			///
			[[nodiscard]] std::vector<graphics::Mesh>& get_meshes() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			Model(const Model&) = delete;

			///
			/// Copy assignment operator.
			///
			Model& operator=(const Model&) = delete;

			///
			/// Parse assimp node.
			///
			/// \param node Node to parse.
			/// \param scene Scene node belongs to.
			///
			void parse_node(aiNode* node, const aiScene* scene) noexcept;

			///
			/// Parse an assimp mesh to galaxy mesh.
			///
			/// \param mesh Assimp mesh to parse.
			/// \param scene Scene mesh belongs to.
			///
			/// \return Unique pointer to galaxy mesh. Must be moved.
			///
			[[nodiscard]] graphics::Mesh parse_mesh(aiMesh* mesh, const aiScene* scene) noexcept;

			///
			/// Parse material texture.
			///
			/// \param mat Assimp material.
			/// \param type Assimp texture type.
			/// \param material Pointer to material in MaterialBook.
			///
			void parse_material_texture(aiMaterial* mat, aiTextureType type, light::Material* material);

		private:
			///
			/// Model file.
			///
			std::string m_file;

			///
			/// Container of unique pointers for meshes.
			///
			std::vector<graphics::Mesh> m_meshes;
		};
	} // namespace components
} // namespace galaxy

#endif