///
/// OpenGLError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LOGGING_OPENGLERROR_HPP_
#define GALAXY_LOGGING_OPENGLERROR_HPP_

#include <string>
#include <vector>

namespace galaxy
{
	///
	/// Convert a GL error code to a string.
	///
	/// \param code Code to convert.
	///
	/// \return String containing error message for that code.
	///
	[[nodiscard]]
	std::string gl_errcode_as_string(const int code) noexcept;

	///
	/// Returns all GL errors on stack.
	///
	/// \return All current code errors, as strings to get definition.
	///
	[[nodiscard]]
	std::vector<std::string> gl_get_all_errors() noexcept;

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
	void gl_add_error(const unsigned int source, const unsigned int type, const unsigned int id, const unsigned int severity, const int length, const char* buf) noexcept;
} // namespace galaxy

#endif
