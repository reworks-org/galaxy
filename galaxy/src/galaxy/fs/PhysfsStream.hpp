///
/// PhysfsStream.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_PHYSFSSTREAM_HPP_
#define GALAXY_FS_PHYSFSSTREAM_HPP_

#include <string>

#include <SFML/System/InputStream.hpp>

struct PHYSFS_File;

namespace galaxy
{
	namespace fs
	{
		///
		/// Custom SFML input stream using PhysFS.
		///
		class PhysfsStream : public sf::InputStream
		{
		public:
			///
			/// Constructor.
			///
			PhysfsStream() noexcept;

			///
			/// Argument constructor.
			///
			/// Does not actually read any data.
			///
			/// \param filename Name of file in VFS to read.
			///
			PhysfsStream(const std::string& filename) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~PhysfsStream() noexcept;

			///
			/// Is the physfs file open.
			///
			/// \return True if open.
			///
			[[nodiscard]]
			bool is_open() const noexcept;

			///
			/// \brief Open file in physfs.
			///
			/// Does not actually read any data.
			///
			/// \param filename Name of file in VFS to read.
			///
			/// \return True if successful.
			///
			[[maybe_unused]]
			bool open(const std::string& filename) noexcept;

			///
			/// Close the open physfs handle.
			///
			void close() noexcept;

			///
			/// \brief Read data from the stream
			///
			/// After reading, the stream's reading position must be
			/// advanced by the amount of bytes read.
			///
			/// \param data Buffer where to copy the read data.
			/// \param size Desired number of bytes to read.
			///
			/// \return The number of bytes actually read, or std::nullopt on error.
			///
			[[nodiscard]]
			std::optional<std::size_t> read(void* data, std::size_t size) override;

			///
			/// Change the current reading position.
			///
			/// \param position The position to seek to, from the beginning.
			///
			/// \return The position actually sought to, or std::nullopt on error.
			///
			[[nodiscard]]
			std::optional<std::size_t> seek(std::size_t position) override;

			///
			/// Get the current reading position in the stream.
			///
			/// \return The current position, or std::nullopt on error.
			///
			[[nodiscard]]
			std::optional<std::size_t> tell() override;

			///
			/// Return the size of the stream
			///
			/// \return The total number of bytes available in the stream, or std::nullopt on error.
			///
			[[nodiscard]]
			std::optional<std::size_t> getSize() override;

		private:
			///
			/// Copy constructor.
			///
			PhysfsStream(const PhysfsStream&) = delete;

			///
			/// Copy assignment operator.
			///
			PhysfsStream& operator=(const PhysfsStream&) = delete;

			///
			/// Move constructor.
			///
			PhysfsStream(PhysfsStream&&) = delete;

			///
			/// Move assignment operator.
			///
			PhysfsStream& operator=(PhysfsStream&&) = delete;

		private:
			///
			/// Handle to physfs file data.
			///
			PHYSFS_File* m_file;
		};
	} // namespace fs
} // namespace galaxy
#endif
