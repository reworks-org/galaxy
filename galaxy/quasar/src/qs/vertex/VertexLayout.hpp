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

#include "qs/utils/Meta.hpp"
#include "qs/vertex/VertexAttribute.hpp"
#include "qs/vertex/type/SpriteVertex.hpp"
#include "qs/vertex/type/PrimitiveVertex.hpp"

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
		template<typename VertexType, typename VertexAttribute>
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

	template<typename VertexType, typename VertexAttribute>
	inline void VertexLayout::add(const int size) noexcept
	{
		// If not one of the four attribute type structs, throw compile-time assert.
		static_assert(std::is_same<VertexAttribute, qs::VATypePosition>::value ||
					  std::is_same<VertexAttribute, qs::VATypeColour>::value ||
					  std::is_same<VertexAttribute, qs::VATypeTexel>::value ||
				      std::is_same<VertexAttribute, qs::VATypeOpacity>::value
		);

		// If not one of the two buffer type structs, throw compile-time assert.
		static_assert(std::is_same<VertexType, qs::SpriteVertex>::value ||
					  std::is_same<VertexType, qs::PrimitiveVertex>::value
		);

		// Now to use constexpr to check on compile time the buffer type.
		// This is faster since we dont need to bother checking at runtime.
		// constexpr will discard the branch that is false and it wont be compiled.
		if constexpr (std::is_same<VertexType, qs::SpriteVertex>::value)
		{
			if constexpr (std::is_same<VertexAttribute, qs::VATypePosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_pos));
			}
			else if constexpr (std::is_same<VertexAttribute, qs::VATypeTexel>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_texels));
			}
			else if constexpr (std::is_same<VertexAttribute, qs::VATypeOpacity>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::SpriteVertex, m_opacity));
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed to add vertex layout type for sprite vertex.");
			}
		}
		else if constexpr (std::is_same<VertexType, qs::PrimitiveVertex>::value)
		{
			if constexpr (std::is_same<VertexAttribute, qs::VATypePosition>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::PrimitiveVertex, m_pos));
			}
			else if constexpr (std::is_same<VertexAttribute, qs::VATypeColour>::value)
			{
				m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, offsetof(qs::PrimitiveVertex, m_colour));
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed to add vertex layout type for primitive vertex.");
			}
		}
	}
}

#endif