///
/// Error.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_ERROR_HPP_
#define FRB_ERROR_HPP_

#include <string>

namespace frb
{
	///
	/// Processess for an OpenAL error.
	///
	/// \param message Message to print alongside OpenAL error.
	///
	const std::string parseError(const std::string& message) noexcept;
}

#endif