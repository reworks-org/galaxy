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
#include "galaxy/graphics/vertex/type/PrimitiveVertex.hpp"
#include "galaxy/graphics/vertex/type/SpriteVertex.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// \brief Specifies "position" vertex attribute type at compile time.
		///
		/// Cannot be instansiated.
		///
		struct VAPosition final
		{
			VAPosition() = delete;
		};

		///
		/// \brief Specifies "colour" vertex attribute type at compile time.
		///
		/// Cannot be instansiated.
		///
		struct VAColour final
		{
			VAColour() = delete;
		};

		///
		/// \brief Specifies "texel" vertex attribute type at compile time.
		///
		/// Cannot be instansiated.
		///
		struct VATexel final
		{
			VATexel() = delete;
		};

		///
		/// \brief Specifies "instance offset" vertex attribute type at compile time.
		///
		/// Cannot be instansiated.
		///
		struct VAInstanceOffset final
		{
			VAInstanceOffset() = delete;
		};

		///
		/// \brief Specifies "opacity" vertex attribute type at compile time.
		///
		/// Cannot be instansiated.
		///
		struct VAOpacity final
		{
			VAOpacity() = delete;
		};

		///
		/// Concept to ensure template parameter is a vertex attribute.
		///
		template<typename Type>
		concept is_vertex_attribute = (std::is_same<Type, VAPosition>::value ||
					       std::is_same<Type, VAColour>::value ||
					       std::is_same<Type, VATexel>::value ||
					       std::is_same<Type, VAInstanceOffset>::value ||
					       std::is_same<Type, VAOpacity>::value);

		///
		/// Concept to ensure template parameter is a vertex.
		///
		template<typename Type>
		concept is_vertex = (std::is_same<Type, graphics::SpriteVertex>::value ||
				     std::is_same<Type, graphics::PrimitiveVertex>::value);

		///
		/// Allows instance buffer to be used as a vertex attribute array.
		///
		template<typename Type>
		concept is_vertex_or_instance = (is_vertex<Type> || std::is_same<Type, graphics::InstanceBuffer>::value);

	} // namespace meta
} // namespace galaxy

#endif