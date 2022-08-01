///
/// AudioFileReader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_AUDIOFILEREADER_HPP_
#define GALAXY_AUDIO_AUDIOFILEREADER_HPP_

#include <string>

#include "irrKlang.h"

namespace galaxy
{
	namespace audio
	{
		///
		/// A file reader interface for galaxy to provide correct vfs paths to irrklang.
		///
		class AudioFileReader final : public irrklang::IFileReader
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param opened_file Pointer to opened file on disk.
			/// \param filename Name of file to open.
			///
			AudioFileReader(FILE* opened_file, const irrklang::ik_c8* filename) noexcept;

			///
			/// Destructor.
			///
			virtual ~AudioFileReader() noexcept;

			///
			/// Reads an amount of bytes from the file.
			///
			/// \param buffer Pointer to buffer where to read bytes will be written to.
			/// \param size_to_read Amount of bytes to read from the file.
			///
			/// \return Returns how much bytes were read.
			///
			[[nodiscard]] irrklang::ik_s32 read(void* buffer, irrklang::ik_u32 size_to_read) override;

			///
			/// Changes position in file, returns true if successful.
			///
			/// \param final_pos Destination position in the file.
			/// \param relative_movement If set to true, the position in the file is
			/// changed relative to current position. Otherwise the position is changed
			/// from beginning of file.
			///
			/// \return Returns true if successful, otherwise false.
			///
			[[nodiscard]] bool seek(irrklang::ik_s32 final_pos, bool relative_movement) override;

			///
			/// Returns size of file.
			///
			/// \return Returns the size of the file in bytes.
			///
			[[nodiscard]] irrklang::ik_s32 getSize() override;

			///
			/// Returns the current position in the file.
			///
			/// \return Returns the current position in the file in bytes.
			///
			[[nodiscard]] irrklang::ik_s32 getPos() override;

			///
			/// Returns name of file.
			///
			/// \return Returns the file name as zero terminated character string.
			///
			[[nodiscard]] const irrklang::ik_c8* getFileName() override;

		private:
			///
			/// \brief Constructor.
			///
			/// Deleted.
			///
			AudioFileReader() = delete;

		private:
			///
			/// Internal pointer to file.
			///
			FILE* m_file;

			///
			/// File name buffer.
			///
			std::string m_filename;

			///
			/// File size.
			///
			irrklang::ik_s32 m_filesize;
		};
	} // namespace audio
} // namespace galaxy

#endif