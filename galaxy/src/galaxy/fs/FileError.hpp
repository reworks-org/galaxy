///
/// FileError.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_FILEERROR_HPP_
#define GALAXY_FS_FILEERROR_HPP_

#include <filesystem>
#include <format>

namespace galaxy
{
	///
	/// Stores information about a File I/O error.
	///
	class FileError final
	{
	public:
		///
		/// Constructor.
		///
		FileError() noexcept;

		///
		/// Argument constructor.
		///
		/// \param mode File I/O i.e. read/write.
		/// \param reason Error failure.
		/// \param path Path where I/O failed.
		///
		FileError(const std::string& mode, const std::string& reason, const std::filesystem::path& path) noexcept;

		///
		/// Destructor.
		///
		~FileError() noexcept;

		///
		/// Prints the file error in a nice format to the console.
		///
		void log() const noexcept;

	public:
		///
		/// File I/O mode.
		///
		std::string m_mode;

		///
		/// Reason for file i/o failure.
		///
		std::string m_reason;

		///
		/// Path error occured on.
		///
		std::filesystem::path m_path;
	};
} // namespace galaxy

template<>
struct std::formatter<std::filesystem::path> : std::formatter<std::string>
{
	auto format(const std::filesystem::path& path, format_context& ctx) const
	{
		return std::formatter<std::string>::format(path.string(), ctx);
	}
};

template<>
struct std::formatter<galaxy::FileError> : std::formatter<std::string>
{
	auto format(const galaxy::FileError& fe, format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("Failed to {0} {2} because {1}", fe.m_mode, fe.m_reason, fe.m_path), ctx);
	}
};

#endif
