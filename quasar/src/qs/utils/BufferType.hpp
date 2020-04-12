///
/// BufferType.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_BUFFERTYPE_HPP_
#define QUASAR_BUFFERTYPE_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// Enum for STATIC or DYNAMIC buffer.
	///
	enum class BufferType : short
	{
		///
		/// VertexBuffer cannot be changed.
		///
		STATIC,

		///
		/// VertexBuffer can be changed.
		///
		DYNAMIC
	};
}

#endif