///
/// AudioFileFactory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_AUDIOFILEFACTORY_HPP_
#define GALAXY_AUDIO_AUDIOFILEFACTORY_HPP_

#include "irrKlang.h"

namespace galaxy
{
	namespace audio
	{
		///
		/// Custom file handler for irrKlang to integrate it with the VFS.
		///
		class AudioFileFactory final : public irrklang::IFileFactory
		{
		public:
			///
			/// Constructor.
			///
			AudioFileFactory() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~AudioFileFactory() noexcept = default;

			///
			/// Opens a file for read access.
			///
			/// \param filename Name of file to open.
			///
			/// \return Returns a pointer to the created file interface.
			///
			[[nodiscard]] irrklang::IFileReader* createFileReader(const irrklang::ik_c8* filename) override;
		};
	} // namespace audio
} // namespace galaxy

#endif