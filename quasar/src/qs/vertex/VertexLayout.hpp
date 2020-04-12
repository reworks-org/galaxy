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

#include "VertexAttribute.hpp"

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
		/// Adds a vertex attribute to the layout.
		///
		/// \param size Number of components for each vertex attribute.
		/// \param type Type of vertex member the attribute is for.
		///
		void add(const int size, const qs::VertexAttribute::Type type);

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
}

#endif