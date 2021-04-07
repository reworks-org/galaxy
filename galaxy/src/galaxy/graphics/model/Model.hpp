///
/// Model.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_MODEL_MODEL_HPP_
#define GALAXY_GRAPHICS_MODEL_MODEL_HPP_

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "galaxy/graphics/model/Mesh.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Model final
		{
		public:
			Model() noexcept;
			Model(std::string_view file);

			[[nodiscard]] const bool load(std::string_view file);
			void create();

			void draw(Shader* shader);

		private:
			void parse_node(aiNode* node, const aiScene* scene) noexcept;
			[[nodiscard]] std::unique_ptr<Mesh> parse_mesh(aiMesh* mesh, const aiScene* scene) noexcept;
			void parse_material_texture(aiMaterial* mat, aiTextureType type, TextureMap::Type map_type, TextureMapHolder* output);

		private:
			std::vector<std::unique_ptr<Mesh>> m_meshes;
		};
	} // namespace graphics
} // namespace galaxy

#endif