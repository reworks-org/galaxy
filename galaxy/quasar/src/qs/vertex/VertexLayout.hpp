///
/// VertexLayout.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXLAYOUT_HPP_
#define QUASAR_VERTEXLAYOUT_HPP_

#include <vector>

#include <glad/glad.h>

#include "qs/utils/Meta.hpp"
#include "qs/vertex/Vertex.hpp"
#include "qs/vertex/VertexAttribute.hpp"

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
		/// VertexAttributeType is the type of vertex member the attribute is for.
		///
		/// \param size Number of components for each vertex attribute.
		///
		template<typename VertexAttributeType>
		void add(const int size) noexcept;

		///
		/// Retrieve all attributes.
		///
		/// \return const reference to std::vector<qs::VertexAttribute>.
		///
		const std::vector<qs::VertexAttribute>& getAttributes() const noexcept;

	private:
		///
		/// Container for all the attributes of this layout.
		///
		std::vector<qs::VertexAttribute> m_attributes;
	};

	template<typename VertexAttributeType>
	inline void VertexLayout::add(const int size) noexcept
	{
		// If not one of the two buffer type structs, throw compile-time assert.
		static_assert(std::is_same<VertexAttributeType, qs::VATypePosition>::value || 
					  std::is_same<VertexAttributeType, qs::VATypeColour>::value ||
			          std::is_same<VertexAttributeType, qs::VATypeTexel>::value
		);

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<VertexAttributeType, qs::VATypePosition>::value)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_position));
		}
		else if constexpr (std::is_same<VertexAttributeType, qs::VATypeColour>::value)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_colour));
		}
		else if constexpr (std::is_same<VertexAttributeType, qs::VATypeTexel>::value)
		{
			m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::Vertex, m_texels));
		}
		else
		{
			throw std::runtime_error("How did you even get here???");
		}
	}
}

#endif