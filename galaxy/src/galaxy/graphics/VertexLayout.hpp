///
/// VertexLayout.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXLAYOUT_HPP_
#define GALAXY_GRAPHICS_VERTEXLAYOUT_HPP_

#include <vector>

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/graphics/Vertex.hpp"
#include "galaxy/graphics/VertexAttribute.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Container for vertex attributes of verticies in order to specify layout of vertexs in their array.
		///
		class VertexLayout final
		{
		public:
			///
			/// Constructor.
			///
			VertexLayout() noexcept = default;

			///
			/// Move constructor.
			///
			VertexLayout(VertexLayout&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexLayout& operator=(VertexLayout&&) noexcept;

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
			template<VertexAttributes va>
			void add(const int size) noexcept;

			///
			/// Retrieve all attributes.
			///
			/// \return const reference to std::vector<VertexAttribute>.
			///
			[[nodiscard]] const std::vector<VertexAttribute>& get_attributes() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			VertexLayout(const VertexLayout&) = delete;

			///
			/// Copy assignment operator.
			///
			VertexLayout& operator=(const VertexLayout&) = delete;

		private:
			///
			/// Container for all the attributes of this layout.
			///
			std::vector<VertexAttribute> m_attributes;
		};

		template<VertexAttributes va>
		inline void VertexLayout::add(const int size) noexcept
		{
			// Now to use constexpr to check on compile time the buffer type.
			// This is faster since we dont need to bother checking at runtime.
			// constexpr will discard the branch that is false and it wont be compiled.

			if constexpr (va == VertexAttributes::POSITION)
			{
				m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), offsetof(Vertex, m_pos));
			}
			else if constexpr (va == VertexAttributes::TEXEL)
			{
				m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), offsetof(Vertex, m_texels));
			}
			else if constexpr (va == VertexAttributes::COLOUR)
			{
				m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), offsetof(Vertex, m_colour));
			}
			else
			{
				static_assert(false, "Invalid vertex attribute specificed.");
			}
		}
	} // namespace graphics
} // namespace galaxy

#endif