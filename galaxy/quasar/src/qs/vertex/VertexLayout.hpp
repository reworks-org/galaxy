///
/// VertexLayout.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXLAYOUT_HPP_
#define QUASAR_VERTEXLAYOUT_HPP_

#include <vector>

#include <glad/glad.h>
#include <pulsar/Log.hpp>

#include "qs/vertex/VertexAttribute.hpp"
#include "qs/utils/Meta.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Container for vertex attributes of verticies in order to specify layout of vertexs in their array.
	///
	class VertexLayout final
	{
	public:
		///
		/// Default constructor.
		///
		VertexLayout() noexcept;

		///
		/// Copy constructor.
		///
		VertexLayout(const VertexLayout&) noexcept = default;

		///
		/// Move constructor.
		///
		VertexLayout(VertexLayout&&);

		///
		/// Copy assignment operator.
		///
		VertexLayout& operator=(const VertexLayout&) noexcept = default;

		///
		/// Move assignment operator.
		///
		VertexLayout& operator=(VertexLayout&&);

		///
		/// Destructor.
		///
		~VertexLayout() noexcept;

		///
		/// \brief Adds a vertex attribute to the layout.
		///
		/// VertexType Is the type of vertex having an vertex attribute added.
		/// VertexAttributeType is the type of vertex member the attribute is for.
		///
		/// \param size Number of components for each vertex attribute.
		///
		template<is_vertex VertexType, is_vertex_attribute VertexAttribute>
		void add(const unsigned int size);

		///
		/// Retrieve all attributes.
		///
		/// \return const reference to std::vector<VertexAttribute>.
		///
		[[nodiscard]] const std::vector<VertexAttribute>& get_attributes() const noexcept;

	private:
		///
		/// Container for all the attributes of this layout.
		///
		std::vector<VertexAttribute> m_attributes;
	};

	template<is_vertex VertexType, is_vertex_attribute VertexAttribute>
	inline void VertexLayout::add(const unsigned int size)
	{
		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		constexpr bool is_instanced_vertex = std::is_same<VertexType, InstanceVertex>::value;
		constexpr bool is_prim_vertex      = std::is_same<VertexType, PrimitiveVertex>::value;
		constexpr bool is_sprite_vertex    = std::is_same<VertexType, SpriteVertex>::value;

		if constexpr (is_sprite_vertex)
		{
			if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, m_pos));
			}
			else if constexpr (std::is_same<VertexAttribute, VATexel>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, m_texels));
			}
			else if constexpr (std::is_same<VertexAttribute, VAOpacity>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(SpriteVertex, m_opacity));
			}
			else
			{
				PL_LOG(PL_FATAL, "Failed to add vertex attribute for sprite vertex.");
			}
		}
		else if constexpr (is_prim_vertex)
		{
			if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(PrimitiveVertex, m_pos));
			}
			else if constexpr (std::is_same<VertexAttribute, VAColour>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(PrimitiveVertex, m_colour));
			}
			else
			{
				PL_LOG(PL_FATAL, "Failed to add vertex attribute for primitive vertex.");
			}
		}
		else if constexpr (is_instanced_vertex)
		{
			if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(InstanceVertex, m_pos));
			}
			else if constexpr (std::is_same<VertexAttribute, VATexel>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(InstanceVertex, m_texels));
			}
			else if constexpr (std::is_same<VertexAttribute, VAInstanceOffset>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(InstanceVertex, m_offset));
			}
			else
			{
				PL_LOG(PL_FATAL, "Failed to add vertex attribute for instanced vertex.");
			}
		}
	}
} // namespace qs

#endif