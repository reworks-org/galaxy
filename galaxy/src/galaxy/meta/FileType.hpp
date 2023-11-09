///
/// FileType.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_META_FILETYPE_HPP_
#define GALAXY_META_FILETYPE_HPP_

#include <span>

#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace meta
	{
		using FSTextR   = std::string;
		using FSTextW   = const std::string&;
		using FSBinaryR = meta::vector<std::uint8_t>;
		using FSBinaryW = std::span<std::uint8_t>;

		///
		/// Ensures proper typing when reading from VFS.
		///
		/// \tparam T Type to test.
		///
		template<typename T>
		concept vfs_read_type = std::is_same_v<FSTextR, T> || std::is_same_v<FSBinaryR, T>;

		///
		/// Ensures proper typing when writing to VFS.
		///
		/// \tparam T Type to test.
		///
		template<typename T>
		concept vfs_write_type = std::is_same_v<FSTextW, T> || std::is_same_v<FSBinaryW, T>;
	} // namespace meta
} // namespace galaxy

#endif
