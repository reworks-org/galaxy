///
/// Model.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>
#include <mutex>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "Model.hpp"

namespace galaxy
{
	namespace graphics
	{
		Model::Model() noexcept
		{
		}

		Model::Model(std::string_view file)
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Model constructor failed to load file: {0}.", file);
			}
			else
			{
				create();
			}
		}

		const bool Model::load(std::string_view file)
		{
			bool result = false;

			const auto abs = SL_HANDLE.vfs()->absolute(file);
			if (abs != std::nullopt)
			{
				static constexpr const auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FindInvalidData | aiProcess_SortByPType | aiProcess_RemoveRedundantMaterials | aiProcess_ImproveCacheLocality;

				Assimp::Importer importer;
				auto scene = importer.ReadFile(abs.value(), flags);
				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to import model: {0}, because: {1}.", file, importer.GetErrorString());
				}
				else
				{
					parse_node(scene->mRootNode, scene);
					result = true;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find model on disk: {0}.", file);
			}

			return result;
		}

		void Model::create()
		{
			for (const auto& mesh : m_meshes)
			{
				mesh->optimize();
				mesh->create();
			}
		}

		void Model::draw(Shader* shader)
		{
			for (const auto& mesh : m_meshes)
			{
				mesh->draw(shader);
			}
		}

		void Model::parse_node(aiNode* node, const aiScene* scene) noexcept
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				auto mesh = scene->mMeshes[node->mMeshes[i]];
				m_meshes.emplace_back(std::move(parse_mesh(mesh, scene)));
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				parse_node(node->mChildren[i], scene);
			}
		}

		std::unique_ptr<Mesh> Model::parse_mesh(aiMesh* mesh, const aiScene* scene) noexcept
		{
			auto output = std::make_unique<Mesh>();

			// Free up memory after, since meshes can be quite large.
			{
				std::vector<unsigned int> count;
				count.resize(mesh->mNumVertices, 0);
				std::iota(count.begin(), count.end(), 0);

				// Use a vector of sequential numbers allows us to parse the verticies in parallel.
				std::mutex mutex;
				std::for_each(std::execution::par, count.begin(), count.end(), [&](const auto& i) {
					Vertex3D vertex;

					if (mesh->HasPositions())
					{
						vertex.m_pos.x = mesh->mVertices[i].x;
						vertex.m_pos.y = mesh->mVertices[i].y;
						vertex.m_pos.z = mesh->mVertices[i].z;
					}

					if (mesh->HasNormals())
					{
						vertex.m_normal.x = mesh->mNormals[i].x;
						vertex.m_normal.y = mesh->mNormals[i].y;
						vertex.m_normal.z = mesh->mNormals[i].z;
					}

					if (mesh->HasTextureCoords(0))
					{
						vertex.m_texels.x = mesh->mTextureCoords[0][i].x;
						vertex.m_texels.y = mesh->mTextureCoords[0][i].y;
					}
					else
					{
						vertex.m_texels.x = 0.0f;
						vertex.m_texels.y = 0.0f;
					}

					vertex.set_index(i);

					// Only shared resource.
					mutex.lock();
					output->m_verticies.emplace_back(vertex);
					mutex.unlock();
				});

				// Sort verticies afterwards to ensure correct order.
				// No need for stable sort, each index is guaranteed to be different.
				std::sort(std::execution::par, output->m_verticies.begin(), output->m_verticies.end(), [&](const auto& left, const auto& right) {
					return left.get_index() < right.get_index();
				});
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				const auto& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
				{
					output->m_indicies.push_back(face.mIndices[j]);
				}
			}

			if (mesh->mMaterialIndex >= 0)
			{
				auto* material = scene->mMaterials[mesh->mMaterialIndex];

				aiColor4D amb_color = {};
				if (material->Get(AI_MATKEY_COLOR_AMBIENT, amb_color) != aiReturn_SUCCESS)
				{
					output->m_material.m_ambient = glm::vec3 {1.0f};
				}
				else
				{
					output->m_material.m_ambient.x = amb_color.r;
					output->m_material.m_ambient.y = amb_color.g;
					output->m_material.m_ambient.z = amb_color.b;
				}

				aiColor4D diff_color = {};
				if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diff_color) != aiReturn_SUCCESS)
				{
					output->m_material.m_ambient = glm::vec3 {1.0f};
				}
				else
				{
					output->m_material.m_diffuse.x = diff_color.r;
					output->m_material.m_diffuse.y = diff_color.g;
					output->m_material.m_diffuse.z = diff_color.b;
				}

				aiColor4D spec_color = {};
				if (material->Get(AI_MATKEY_COLOR_SPECULAR, spec_color) != aiReturn_SUCCESS)
				{
					output->m_material.m_ambient = glm::vec3 {0.5f};
				}
				else
				{
					output->m_material.m_specular.x = spec_color.r;
					output->m_material.m_specular.y = spec_color.g;
					output->m_material.m_specular.z = spec_color.b;
				}

				if (material->Get(AI_MATKEY_SHININESS, output->m_material.m_shininess) != aiReturn_SUCCESS)
				{
					output->m_material.m_shininess = 32.0f;
				}

				parse_material_texture(material, aiTextureType_DIFFUSE, TextureMap::Type::DIFFUSE, &output->m_textures);
				parse_material_texture(material, aiTextureType_SPECULAR, TextureMap::Type::SPECULAR, &output->m_textures);
				parse_material_texture(material, aiTextureType_HEIGHT, TextureMap::Type::NORMAL, &output->m_textures);
				parse_material_texture(material, aiTextureType_AMBIENT, TextureMap::Type::HEIGHT, &output->m_textures);
			}

			return output;
		}

		void Model::parse_material_texture(aiMaterial* mat, aiTextureType type, TextureMap::Type map_type, TextureMapHolder* output)
		{
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);

				const auto filename = std::filesystem::path(str.C_Str()).filename().string();
				if (!output->contains(filename))
				{
					TextureMap texture;
					const auto abs = SL_HANDLE.vfs()->absolute(filename);

					if (abs != std::nullopt)
					{
						texture.m_texture.load(abs.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Failed to load model texture: {0}.", filename);
					}

					texture.m_type = map_type;
					output->emplace(filename, std::move(texture));
				}
			}
		}
	} // namespace graphics
} // namespace galaxy