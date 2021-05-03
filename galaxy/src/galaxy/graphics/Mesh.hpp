///
/// Mesh.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_MESH_HPP_
#define GALAXY_GRAPHICS_MESH_HPP_

#include "galaxy/graphics/vertex/Vertex3D.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
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
			/// Move constructor.
			///
			Mesh(Mesh&&) noexcept;

			///
			/// Move assignment operator.
			///
			Mesh& operator=(Mesh&&) noexcept;

			///
			/// Destructor.
			///
			~Mesh() noexcept;

			///
			/// Create vertex array object.
			/// And also calls optimize().
			///
			void create();

			///
			/// Bind mesh VAO.
			///
			void bind() noexcept;

			///
			/// Unbind mesh VAO.
			///
			void unbind() noexcept;

			///
			/// Get VAO index count.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int index_count() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Mesh(const Mesh&) = delete;

			///
			/// Copy assignment operator.
			///
			Mesh& operator=(const Mesh&) = delete;

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
			/// Material ID.
			///
			std::string m_material_id;
		};
	} // namespace graphics
} // namespace galaxy

#endif