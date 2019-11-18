///
/// VertexLayout.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXLAYOUT_HPP_
#define QUASAR_VERTEXLAYOUT_HPP_

#include <vector>

#include "qs/utils/Error.hpp"
#include "qs/libs/glad/glad.h"

#include "VertexAttribute.hpp"

namespace qs
{
	///
	/// Container for vertex attributes of verticies in order to specify layout of verticies in their array.
	///
	class VertexLayout final
	{
	public:
		///
		/// Constructor.
		///
		VertexLayout(unsigned int stride) noexcept;

		///
		/// Destructor.
		///
		~VertexLayout() noexcept;

		///
		/// Adds a vertex attribute to the layout.
		///
		/// \param size Number of components for each vertex attribute.
		///
		template<typename Type>
		void add(int size) noexcept;
		
		///
		/// Retrieve all attributes.
		///
		/// \return const reference to std::vector<qs::VertexAttribute>.
		///
		const std::vector<qs::VertexAttribute>& getAttributes() const noexcept;

		///
		/// Retrieve stride.
		///
		/// \return const unsigned int of stride.
		///
		const unsigned int stride() const;

	private:
		///
		/// Default constructor.
		///
		VertexLayout() = delete;

	private:
		///
		/// Container for all the attributes of this layout.
		///
		std::vector<qs::VertexAttribute> m_attributes;

		///
		/// Stores the stride for the layout - total vertex mappings per row.
		///
		unsigned int m_stride;

		///
		/// Stores current total offset.
		///
		unsigned int m_totalOffset;
	};

	template<typename Type>
	inline void VertexLayout::add(int size) noexcept
	{
		qs::Error::handle.callback("VertexLayout.hpp", 74, "Unsupported vertex attribute type!");
	}

	template<>
	inline void VertexLayout::add<float>(int size) noexcept
	{
		// First offset is 0.
		m_attributes.emplace_back(size, GL_FLOAT, GL_FALSE, m_totalOffset * sizeof(GLfloat));
		m_totalOffset += size;
	}

	template<>
	inline void VertexLayout::add<unsigned int>(int size) noexcept
	{
		// First offset is 0.
		m_attributes.emplace_back(size, GL_UNSIGNED_INT, GL_FALSE, m_totalOffset * sizeof(GLuint));
		m_totalOffset += size;
	}
}

#endif