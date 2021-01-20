///
/// Music.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_MUSIC_HPP_
#define GALAXY_AUDIO_MUSIC_HPP_

#include <thread>

#include <nlohmann/json_fwd.hpp>

#include "galaxy/audio/source/Source.hpp"
#include "galaxy/audio/source/SourceManipulator.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// \brief Streamed audio source.
		///
		class Music final : public BufferStream, public SourceManipulator
		{
		public:
			///
			/// Constructor.
			///
			Music();

			///
			/// Argument constructor.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			Music(std::string_view file);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Music(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Music(Music&&);

			///
			/// Move assignment operator.
			///
			Music& operator=(Music&&);

			///
			/// Destructor.
			///
			virtual ~Music();

			///
			/// Load a file to stream from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] bool load(std::string_view file);

		private:
			///
			/// Copy assignment operator.
			///
			Music& operator=(const Music&) = delete;

			///
			/// Copy constructor.
			///
			Music(const Music&) = delete;

		private:
			///
			/// Update stream buffers as it plays.
			///
			void update();

			///
			/// OpenAL audio source object.
			///
			Source m_source;

			///
			/// Thread to process music updates on.
			///
			std::jthread m_thread;

			///
			/// Thread control.
			///
			std::atomic_bool m_running;
		};
	} // namespace audio
} // namespace galaxy

#endif