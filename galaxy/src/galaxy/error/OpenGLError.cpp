///
/// OpenGLError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "OpenGLError.hpp"

namespace galaxy
{
	namespace error
	{
		std::string gl_errcode_as_string(const int code)
		{
			switch (code)
			{
				case GL_INVALID_ENUM:
					return "GL_INVALID_ENUM";
					break;

				case GL_INVALID_VALUE:
					return "GL_INVALID_VALUE";
					break;

				case GL_INVALID_OPERATION:
					return "GL_INVALID_OPERATION";
					break;

				case GL_STACK_OVERFLOW:
					return "GL_STACK_OVERFLOW";
					break;

				case GL_STACK_UNDERFLOW:
					return "GL_STACK_UNDERFLOW";
					break;

				case GL_OUT_OF_MEMORY:
					return "GL_OUT_OF_MEMORY";
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					return "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;

				default:
					return "GL_NO_ERROR";
					break;
			}
		}

		meta::vector<std::string> gl_get_all_errors()
		{
			meta::vector<std::string> errors;

			GLenum code;
			while ((code = glGetError()) != GL_NO_ERROR)
			{
				errors.emplace_back(gl_errcode_as_string(code));
			}

			return errors;
		}

		void gl_add_error(const unsigned int source,
			const unsigned int type,
			const unsigned int id,
			const unsigned int severity,
			const int length,
			const char* buf)
		{
			glDebugMessageInsert(source, type, id, severity, length, buf);
		}
	} // namespace error
} // namespace galaxy