///
/// StorageFlags.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_STORAGEFLAG_HPP_
#define GALAXY_GRAPHICS_STORAGEFLAG_HPP_

#include <glad/glad.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// C++ style enum for OpenGL storage flags.
		///
		enum class StorageFlag : GLenum
		{
			STATIC_DRAW     = GL_STATIC_DRAW,
			DYNAMIC_DRAW    = GL_DYNAMIC_DRAW
		};
	} // namespace graphics
} // namespace galaxy

#endif