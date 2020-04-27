///
/// StringOps.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_STRINGOPS_HPP_
#define PROTOSTAR_STRINGOPS_HPP_

#include <ios>
#include <string>

///
/// Core namespace.
///
namespace protostar
{
	//
	/// Converts a boolean to a std::string.
	///
	/// \param value Boolean value to convert.
	///
	/// \return Returns either "true" or "false".
	///
	const std::string boolToString(const bool value);

	///
	/// Converts a std::string to a boolean.
	///
	/// \param str std::string value to convert.
	///
	/// \return Returns either true or false.
	///
	const bool stringToBool(const std::string& str);

	///
	/// \brief Ensures any null c-strings are converted to empty ones.
	///
	/// Credits: https://stackoverflow.com/a/1724514
	///
	/// \param s String to check.
	///
	/// \return Returns empty string if input is null.
	///
	const char* nullToEmpty(const char* s);

	///
	/// \brief Convert a std::string to any numerical type.
	///
	/// Credits: https://gist.github.com/timofurrer/2725779
	/// Some adjustments have been made.
	///
	/// \param data std::string containing a numerical type to convert.
	///
	/// \return Returns data as T type.
	///
	template <typename T>
	inline const T convertString(const std::string& data)
	{
		// Make sure is arimthmatic.
		static_assert(std::is_arithmetic<T>::value);

		if (!data.empty())
		{
			T ret = (T)0;

			// Create stringstream and use it to convert strings to variables.
			std::istringstream iss(data);
			if (data.find("0x") != std::string::npos)
			{
				iss >> std::hex >> ret;
			}
			else
			{
				iss >> std::dec >> ret;
			}

			if (iss.fail())
			{
				return T();
			}

			return ret;
		}

		return T();
	}
}

#endif