///
/// Meta.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_META_HPP_
#define QUASAR_META_HPP_

#include <type_traits>

#include "qs/vertex/type/PrimitiveVertex.hpp"
#include "qs/vertex/type/SpriteVertex.hpp"

///
/// Core namespace.
///
namespace qs
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
	/// \brief Specifies "opacity" vertex attribute type at compile time.
	///
	/// Cannot be instansiated.
	///
	struct VAOpacity final
	{
		VAOpacity() = delete;
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
	/// Static function to easily create a vertex.
	///
	/// \param args Arguments for vertex type constructor.
	///
	/// \return Vertex.
	///
	template<typename vertex_type, typename... _args>
	[[nodiscard]] inline vertex_type make_vertex(_args&&... args) noexcept
	{
		return {std::forward<_args>(args)...};
	}

	///
	/// Concept to ensure template parameter is a vertex attribute.
	///
	template<typename type>
	concept is_vertex_attribute = (std::is_same<type, VAPosition>::value ||
				       std::is_same<type, VAColour>::value ||
				       std::is_same<type, VATexel>::value ||
				       std::is_same<type, VAOpacity>::value);

	///
	/// Concept to ensure template parameter is a vertex.
	///
	template<typename type>
	concept is_vertex = (std::is_same<type, SpriteVertex>::value ||
			     std::is_same<type, PrimitiveVertex>::value);

	///
	/// Conceptto ensure template parameter is a opengl buffer type.
	///
	template<typename type>
	concept is_buffer = (std::is_same<type, BufferDynamic>::value || std::is_same<type, BufferStatic>::value);

} // namespace qs

#endif