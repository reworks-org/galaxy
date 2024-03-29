///
/// OpenGLError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ERROR_OPENGLERROR_HPP_
#define GALAXY_ERROR_OPENGLERROR_HPP_

#include <string>

#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace error
	{
		///
		/// Convert a GL error code to a string.
		///
		/// \param code Code to convert.
		///
		/// \return Code as a string.
		///
		[[nodiscard]] std::string gl_errcode_as_string(const int code);

		///
		/// Returns all GL errors on stack.
		///
		/// \return All current errors on stack in an array, as a string.
		///
		[[nodiscard]] meta::vector<std::string> gl_get_all_errors();

		///
		/// Insert a message into the OpenGL debug stack.
		///
		/// \param source The source of the debug message to insert.
		/// \param type The type of the debug message insert.
		/// \param id The user-supplied identifier of the message to insert.
		/// \param severity The severity of the debug messages to insert.
		/// \param length The length string contained in the character array whose address is given by message.
		/// \param buf The address of a character array containing the message to insert.
		///
		void
		gl_add_error(const unsigned int source, const unsigned int type, const unsigned int id, const unsigned int severity, const int length, const char* buf);
	} // namespace error
} // namespace galaxy

#endif
