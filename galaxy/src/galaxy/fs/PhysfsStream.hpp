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
#include <SFML/System/NonCopyable.hpp>

struct PHYSFS_File;

namespace galaxy
{
	namespace fs
	{
		///
		/// SFML <-> Physfs input stream.
		///
		class PhysfsStream : public sf::InputStream
		{
		public:
			///
			/// Argument constructor.
			///
			/// Does not actually read any data.
			///
			/// \param filename Name of file in VFS to read.
			///
			PhysfsStream(const std::string& filename);

			///
			/// Move constructor.
			///
			PhysfsStream(PhysfsStream&&) = delete;

			///
			/// Move assignment operator.
			///
			PhysfsStream& operator=(PhysfsStream&&) = delete;

			///
			/// Virtual destructor.
			///
			virtual ~PhysfsStream();

			///
			/// Is the physfs file open.
			///
			/// \return True if open.
			///
			[[nodiscard]]
			bool is_open() const;

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
			bool open(const std::string& filename);

			///
			/// Close the open physfs handle.
			///
			void close();

			///
			///
			/// \brief Read data from the stream.
			///
			/// After reading, the stream's reading position must be
			/// advanced by the amount of bytes read.
			///
			/// \param data Buffer where to copy the read data.
			/// \param size Desired number of bytes to read.
			///
			/// \return The number of bytes actually read, or -1 on error.
			///
			[[nodiscard]]
			sf::Int64 read(void* data, sf::Int64 size) override;

			///
			/// Change the current reading position
			///
			/// \param position The position to seek to, from the beginning.
			///
			/// \return The position actually sought to, or -1 on error.
			///
			[[maybe_unused]]
			sf::Int64 seek(sf::Int64 position) override;

			///
			/// Get the current reading position in the stream.
			///
			/// \return The current position, or -1 on error.
			///
			[[nodiscard]]
			sf::Int64 tell() override;

			///
			/// Return the size of the stream.
			///
			/// \return The total number of bytes available in the stream, or -1 on error.
			///
			[[nodiscard]]
			sf::Int64 getSize() override;

		private:
			///
			/// Copy constructor.
			///
			PhysfsStream(const PhysfsStream&) = delete;

			///
			/// Copy assignment operator.
			///
			PhysfsStream& operator=(const PhysfsStream&) = delete;

		private:
			///
			/// Handle to physfs file data.
			///
			PHYSFS_File* m_file;
		};
	} // namespace fs
} // namespace galaxy
#endif
