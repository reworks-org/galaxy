///
/// BatchSprite2D.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_BATCHSPRITE2D_HPP_
#define QUASAR_BATCHSPRITE2D_HPP_

#include "qs/core/Texture.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/transforms/Transform.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Batched sprite object.
	/// Contains multiple "sprites" to draw with a single call.
	///
	class BatchSprite2D final : public qs::Texture
	{
	public:
		///
		/// Constructor.
		///
		BatchSprite2D() noexcept;

		///
		/// Destructor.
		///
		~BatchSprite2D() noexcept override;

		///
		///
		///
		void push(const std::array<float, 20>& vertexs) noexcept;

		///
		/// Creates the sprite from provided data.
		///
		/// \param totalSprites Total amount of sprites to be created.
		///
		void create(const int totalSprites) noexcept;

		///
		/// Bind VA and texture.
		///
		void bind() noexcept override;

		///
		/// Gets IndexBuffer count.
		///
		const unsigned int count() const noexcept;

		///
		/// Set an id to be assossiated with name.
		///
		/// \param name String name to use.
		/// \param id Integer ID to use.
		///
		void addID(const std::string& name, const int id) noexcept;

		///
		/// Return the integer ID of a texture in OpenGL from string identifier.
		///
		/// \param name Name of the texture in the atlas.
		///
		/// \return ID as integer.
		///
		const int getID(const std::string& name) noexcept;

		///
		/// \brief Get a transform object for a particular sprite.
		///
		/// Allowed to throw exceptions.
		///
		/// \param name Name of the texture in the atlas.
		///
		/// \return Pointer to a qs::Transform.
		///
		qs::Transform* getTransform(const std::string& name);

		///
		/// \brief Retrieve array of transform to pass to shader.
		///
		/// Allowed to throw exceptions.
		///
		/// \return Newly constructed vector containing all transforms.
		///
		std::vector<glm::mat4>& getTransformArray();

	private:
		///
		/// Counter for stride between quads.
		///
		unsigned int m_counter;
		
		///
		/// Buffer for vertex arrays.
		///
		std::vector<float> m_vBuffer;

		///
		/// Buffer for index buffers.
		///
		std::vector<unsigned int> m_iBuffer;

		///
		/// Hashmap to quickly get a texture from a string name.
		///
		std::unordered_map<std::string, int> m_nameToID;

		///
		/// Stores a set of MV transforms.
		///
		std::vector<qs::Transform> m_transforms;

		///
		/// Stores a set of MV transforms.
		///
		std::vector<glm::mat4> m_rawTransforms;

		///
		/// OpenGL Vertex Buffer Object.
		///
		qs::VertexBuffer m_vertexBuffer;

		///
		/// OpenGL index (element) buffer.
		///
		qs::IndexBuffer m_indexBuffer;

		///
		/// Defines layout of verticies.
		///
		qs::VertexLayout m_layout;

		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_vertexArray;
	};
}

#endif