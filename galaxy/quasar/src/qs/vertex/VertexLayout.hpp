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
		template<is_vertex vertex_type, is_vertex_attribute vertex_attribute>
		void add(const pr::positive_int auto size);

		///
		/// Retrieve all attributes.
		///
		/// \return const reference to std::vector<qs::VertexAttribute>.
		///
		[[nodiscard]] const std::vector<qs::VertexAttribute>& get_attributes() const noexcept;

	private:
		///
		/// Container for all the attributes of this layout.
		///
		std::vector<qs::VertexAttribute> m_attributes;
	};

	template<is_vertex vertex_type, is_vertex_attribute vertex_attribute>
	inline void VertexLayout::add(const pr::positive_int auto size)
	{
		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<vertex_type, qs::SpriteVertex>::value)
		{
			if constexpr (std::is_same<vertex_attribute, qs::VAPosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_pos));
			}
			else if constexpr (std::is_same<vertex_attribute, qs::VATexel>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_texels));
			}
			else if constexpr (std::is_same<vertex_attribute, qs::VAOpacity>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_opacity));
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed to add vertex layout type for sprite vertex.");
			}
		}
		else if constexpr (std::is_same<vertex_type, qs::PrimitiveVertex>::value)
		{
			if constexpr (std::is_same<vertex_attribute, qs::VAPosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::PrimitiveVertex, m_pos));
			}
			else if constexpr (std::is_same<vertex_attribute, qs::VAColour>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::PrimitiveVertex, m_colour));
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed to add vertex layout type for primitive vertex.");
			}
		}
	}
} // namespace qs

#endif