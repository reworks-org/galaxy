///
/// Serializer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZER_HPP_
#define GALAXY_FS_SERIALIZER_HPP_

#include <string_view>

namespace galaxy
{
	namespace fs
	{
		///
		/// Serialize all stored objects.
		///
		/// \param file Path to a json file, including the file.
		///
		void serialize(std::string_view file);

		///
		/// Deserialize a json file.
		///
		/// \param file Path to a json file, including the file.
		///
		void deserialize(std::string_view file);
	} // namespace fs
} // namespace galaxy

#endif