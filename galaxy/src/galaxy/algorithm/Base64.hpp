///
/// Base64.hpp
/// galaxy
///
/// Modified for use in Galaxy Engine.
///

#ifndef GALAXY_ALGORITHM_BASE64_HPP_
#define GALAXY_ALGORITHM_BASE64_HPP_

#include <string>

namespace galaxy
{
	namespace algorithm
	{
		///
		/// \brief Compresses string into Base64.
		///
		/// The MIT License (MIT)
		/// Copyright(c) 2016 tomykaira
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string encode_base64(const std::string& input);

		///
		/// \brief Decompresses string into Base64.
		///
		/// The MIT License (MIT)
		/// Copyright(c) 2016 tomykaira
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string decode_base64(const std::string& input);
	} // namespace algorithm
} // namespace galaxy

#endif