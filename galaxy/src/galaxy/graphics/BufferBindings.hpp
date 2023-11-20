///
/// BufferBindings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_BUFFERBINDINGS_HPP_
#define GALAXY_GRAPHICS_BUFFERBINDINGS_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Stores the location of a buffer's binding point for the VAO.
		///
		enum class BufferBinding : unsigned int
		{
			///
			/// Normal VBO.
			///
			VERTEX_BUFFER_POINT = 0
		};
	} // namespace graphics
} // namespace galaxy

#endif
