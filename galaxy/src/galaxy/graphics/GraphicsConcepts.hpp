///
/// GraphicsConcepts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GRAPHICSCONCEPTS_HPP_
#define GALAXY_GRAPHICS_GRAPHICSCONCEPTS_HPP_

#include <type_traits>

#include "galaxy/graphics/vertex/InstanceBuffer.hpp"
#include "galaxy/graphics/vertex/type/PrimitiveVertex.hpp"
#include "galaxy/graphics/vertex/type/SpriteVertex.hpp"
#include "galaxy/graphics/vertex/type/BatchedVertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Specifies a static buffer at compile time.
		///
		/// Cannot be instansiated.
		///
		struct BufferDynamic final
		{
			BufferDynamic() = delete;
		};

		///
		/// \brief Specifies a dynamic buffer at compile time.
		///
		/// Cannot be instansiated.
		///
		struct BufferStatic final
		{
			BufferStatic() = delete;
		};

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
		/// Function to easily create a vertex.
		///
		/// \param args Arguments for vertex type constructor.
		///
		/// \return Vertex.
		///
		template<typename VertexType, typename... Args>
		[[nodiscard]] inline VertexType make_vertex(Args&&... args)
		{
			return {std::forward<Args>(args)...};
		}

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
		concept is_vertex = (std::is_same<Type, SpriteVertex>::value ||
				     std::is_same<Type, PrimitiveVertex>::value ||
				     std::is_same<Type, BatchedVertex>::value);

		///
		/// Allows instance buffer to be used as a vertex attribute array.
		///
		template<typename Type>
		concept is_vertex_or_instance = (is_vertex<Type> || std::is_same<Type, InstanceBuffer>::value);

		///
		/// Conceptto ensure template parameter is a opengl buffer type.
		///
		template<typename Type>
		concept is_buffer = (std::is_same<Type, BufferDynamic>::value || std::is_same<Type, BufferStatic>::value);

	} // namespace graphics
} // namespace galaxy

#endif