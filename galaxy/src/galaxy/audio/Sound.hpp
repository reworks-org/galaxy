///
/// Sound.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_SOUND_HPP_
#define GALAXY_AUDIO_SOUND_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/audio/source/Source.hpp"
#include "galaxy/audio/source/SourceManipulator.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// \brief Short length ogg-vorbis audio.
		///
		/// Contains a Buffer and Source.
		///
		class Sound final : public Buffer, public SourceManipulator
		{
		public:
			///
			/// Constructor.
			///
			Sound();

			///
			/// Argument constructor.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			Sound(std::string_view file);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Sound(const nlohmann::json& json);

			///
			/// Copy constructor.
			///
			Sound(const Sound&) = delete;

			///
			/// Move constructor.
			///
			Sound(Sound&&);

			///
			/// Copy assignment operator.
			///
			Sound& operator=(const Sound&) = delete;

			///
			/// Move assignment operator.
			///
			Sound& operator=(Sound&&);

			///
			/// Destructor.
			///
			virtual ~Sound() = default;

			///
			/// Load a file from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] bool load(std::string_view file);

		private:
			///
			/// OpenAL audio source.
			///
			Source m_source;
		};
	} // namespace audio
} // namespace galaxy

#endif