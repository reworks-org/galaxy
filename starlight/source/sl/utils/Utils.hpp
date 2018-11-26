///
/// Utils.hpp
/// starlight
///
/// Created by reworks on 26/06/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UTILS_HPP_
#define STARLIGHT_UTILS_HPP_

#include <string>
#include <sstream>
#include <utility>

#include "sl/libs/loguru/loguru.hpp"

namespace sl
{
	///
	/// \brief Used to determine if type or refernce should be returned based on condition.
	///
	/// Base template. Does nothing.
	///
	template<bool condition, class T = void>
	struct ReturnReferenceIfFalse
	{
	};

	///
	/// \brief Used to determine if type or refernce should be returned based on condition.
	///
	/// If true, return plain type.
	///
	template<typename T>
	struct ReturnReferenceIfFalse<true, T>
	{
		typedef T type;
	};

	///
	/// \brief Used to determine if type or refernce should be returned based on condition.
	///
	/// If false, return reference.
	///
	template<typename T>
	struct ReturnReferenceIfFalse<false, T>
	{
		typedef T& type;
	};

	///
	/// Contains static Utility functions.
	///
	struct Utils
	{
		///
		/// Converts a boolean to a std::string.
		///
		/// \param value Boolean value to convert.
		///
		/// \return Returns either "true" or "false".
		///
		static const std::string boolToString(bool value);

		///
		/// Converts a std::string to a boolean.
		///
		/// \param str std::string value to convert.
		///
		/// \return Returns either true or false.
		///
		static const bool stringToBool(const std::string& str);
		
		///
		/// \brief Ensures any null c-strings are converted to empty ones.
		///
		/// Credits: https://stackoverflow.com/a/1724514
		///
		/// \param s String to check.
		///
		/// \return Returns empty string if input is null.
		///
		///
		static const char* nullToEmpty(char const* s);

		///
		/// \brief Do something to each item in tuple.
		///
		/// Different overload.
		/// Credits: https://stackoverflow.com/a/26902803
		/// 
		template<class F, class...Ts, std::size_t...Is>
		static inline void for_each_in_tuple(std::tuple<Ts...> & tuple, F func, std::index_sequence<Is...>)
		{
			// Expands a tuple.
			using expander = int[];
			(void)expander {
				0, ((void)func(std::get<Is>(tuple)), 0)...
			};
		}

		///
		/// \brief Do something to each item in tuple.
		///
		/// E.g. for_each_in_tuple(some, [](const auto &x) { std::cout << x << std::endl; });
		/// Credits: https://stackoverflow.com/a/26902803
		/// 
		template<class F, class...Ts>
		static inline void for_each_in_tuple(std::tuple<Ts...> & tuple, F func)
		{
			// Iterates over a tuple.
			for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
		}

		///
		/// \brief Calculate the percentage in a custom range.
		///
		/// Thanks to: https://stackoverflow.com/a/25835683
		///
		/// \param input The value to check the percentage of.
		/// \param min The value representing 0%.
		/// \param max The value representing 100%.
		/// 
		/// \return Returns the percentage of the value in that range betwen 0 and 1. E.g. 39% is 0.39.
		///
		template<typename T>
		static inline const T customPercentage(T input, T min, T max)
		{
			return ((input - min) / (max - min));
		}

		///
		/// \brief Convert a std::string to any numerical type.
		///
		/// Thanks to: https://gist.github.com/timofurrer/2725779
		/// Some adjustments have been made.
		///
		/// \param data std::string containing a numerical type to convert.
		///
		/// \return Returns data as T type.
		///
		template <typename T>
		static inline const T convertString(const std::string &data)
		{
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
					LOG_S(ERROR) << "Convert error: cannot convert string '" << data << "' to value";
					return T();
				}

				return ret;
			}

			return T();
		}
	};

	///
	/// Template specialization for convertString for std::string.
	///
	/// \see convertString
	///
	template<>
	inline const std::string Utils::convertString<std::string>(const std::string& data)
	{
		return data;
	}

	///
	/// Template specialization for convertString for bool.
	///
	/// \see convertString
	///
	template<>
	inline const bool Utils::convertString<bool>(const std::string& data)
	{
		return Utils::stringToBool(data);
	}
}

#endif