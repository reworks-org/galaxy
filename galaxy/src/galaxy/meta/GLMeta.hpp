///
/// GLMeta.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_GLMETA_HPP_
#define GALAXY_META_GLMETA_HPP_

#include <type_traits>

#include "galaxy/graphics/vertex/InstanceBuffer.hpp"
#include "galaxy/graphics/vertex/BatchVertex.hpp"
#include "galaxy/graphics/vertex/PrimitiveVertex.hpp"
#include "galaxy/graphics/vertex/SpriteVertex.hpp"
#include "galaxy/graphics/vertex/Vertex3D.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Concept to ensure template parameter is a vertex.
		///
		template<typename Type>
		concept is_vertex = (std::is_same<Type, graphics::SpriteVertex>::value ||
				     std::is_same<Type, graphics::PrimitiveVertex>::value ||
				     std::is_same<Type, graphics::BatchVertex>::value ||
				     std::is_same<Type, graphics::Vertex3D>::value);

		///
		/// Allows instance buffer to be used as a vertex attribute array.
		///
		template<typename Type>
		concept is_vertex_or_instance = (is_vertex<Type> || std::is_same<Type, graphics::InstanceBuffer>::value);

	} // namespace meta
} // namespace galaxy

#endif