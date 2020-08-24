///
/// Source.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCE_HPP_
#define FRB_SOURCE_HPP_

#include <span>
#include <vector>

#include "frb/buffer/Buffer.hpp"
#include "frb/buffer/BufferStream.hpp"

///
/// Core namespace.
///
namespace frb
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
		/// Copy constructor.
		///
		Source(const Source&) noexcept = delete;

		///
		/// Move constructor.
		///
		Source(Source&&);

		///
		/// Copy assignment operator.
		///
		Source& operator=(const Source&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Source& operator=(Source&&);

		///
		/// \brief Default destructor.
		///
		/// Destroys source buffer(s).
		///
		~Source();

		///
		/// Queue a buffer for the source to play.
		///
		/// \param buffer Pointer to an frb::Buffer* object. Cannot be nullptr.
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
		[[nodiscard]] const ALuint handle() const noexcept;

	private:
		///
		/// Handle to source.
		///
		ALuint m_source;
	};
} // namespace frb

#endif