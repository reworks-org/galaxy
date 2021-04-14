///
/// Mesh.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_MODEL_MESH_HPP_
#define GALAXY_GRAPHICS_MODEL_MESH_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/light/Material.hpp"
#include "galaxy/graphics/shader/Shader.hpp"
#include "galaxy/graphics/texture/TextureMap.hpp"
#include "galaxy/graphics/vertex/Vertex3D.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		using TextureMapHolder = robin_hood::unordered_flat_map<std::string, TextureMap>;

		///
		/// Mesh object, part of a 3D model.
		///
		class Mesh final : public VertexData
		{
		public:
			///
			/// Constructor.
			///
			Mesh() noexcept = default;

			///
			/// Destructor.
			///
			~Mesh() noexcept;

			///
			/// Create vertex array object.
			///
			void create();

			///
			/// Optimize verticies and indicies for GPU rendering.
			///
			void optimize();

			void draw(Shader* shader);

		public:
			///
			/// Vertex data.
			///
			std::vector<Vertex3D> m_verticies;

			///
			/// Index data.
			///
			std::vector<unsigned int> m_indicies;

			///
			/// Mesh material.
			///
			TextureMapHolder m_textures;

			///
			/// Light material.
			///
			light::Material m_material;
		};
	} // namespace graphics
} // namespace galaxy

#endif