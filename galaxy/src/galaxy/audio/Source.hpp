///
/// Source.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_SOURCE_HPP_
#define GALAXY_AUDIO_SOURCE_HPP_

#include <span>
#include <vector>

#include "galaxy/audio/Buffer.hpp"
#include "galaxy/audio/BufferStream.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// Origin of the audio sound.
		///
		class Source final
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Generates source buffer(s).
			///
			Source();

			///
			/// \brief Default destructor.
			///
			/// Destroys source buffer(s).
			///
			~Source();

			///
			/// Queue a buffer for the source to play.
			///
			/// \param buffer Pointer to an Buffer* object. Cannot be nullptr.
			///
			void queue(Buffer* buffer);

			///
			/// Queue a streamed buffer for the source to play through.
			///
			/// \param stream_buffer Pointer to a BufferStream* object. Cannot be nullptr.
			///
			void queue(BufferStream* stream_buffer);

			///
			/// Queue a set of buffers for the source to play through.
			///
			/// \param buffers Array of buffers you want to play in a queue.
			///
			void queue(std::span<Buffer> buffers);

			///
			/// Raw queue of buffer(s) to play.
			///
			/// \param buffer_array Array of buffers you want to play in a queue.
			///
			void queue(std::span<ALuint> buffer_array);

			///
			/// \brief Get current state of the source.
			///
			/// \return Enum. AL_PLAYING, AL_STOPPED, etc...
			///
			[[nodiscard]] ALint get_state() noexcept;

			///
			/// Get the OpenAL internal int id / handle.
			///
			/// \return ALuint handle integer.
			///
			[[nodiscard]] ALuint handle() const noexcept;

		private:
			///
			/// Move constructor.
			///
			Source(Source&&) = delete;

			///
			/// Move assignment operator.
			///
			Source& operator=(Source&&) = delete;

			///
			/// Copy constructor.
			///
			Source(const Source&) = delete;

			///
			/// Copy assignment operator.
			///
			Source& operator=(const Source&) = delete;

		private:
			///
			/// Handle to source.
			///
			ALuint m_source;
		};
	} // namespace audio
} // namespace galaxy

#endif