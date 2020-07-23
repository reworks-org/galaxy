///
/// Source.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCE_HPP_
#define FRB_SOURCE_HPP_

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
		/// \brief Default destructor.
		///
		/// Destroys source buffer(s).
		///
		~Source() noexcept;

		///
		/// Queue a buffer for the source to play.
		///
		/// \param buffer Buffer to queue.
		///
		void queue(frb::Buffer* buffer);

		///
		/// Queue a streamed buffer for the source to play through.
		///
		/// \param stream_buffer Buffer containing streamed from disk buffers to play.
		///
		void queue(BufferStream* stream_buffer);

		///
		/// Queue a set of buffers for the source to play through.
		///
		/// \param buffers Array of buffers you want to play in a queue.
		///
		void queue(const std::vector<frb::Buffer>& buffers);

		///
		/// Raw queue of buffer(s) to play.
		///
		/// \param buffer_array Array of buffers you want to play in a queue.
		/// \param size The size/length of buffer_array.
		///
		void queue(const ALuint* buffer_array, const size_t size);

		///
		/// \brief Get current state of the source.
		///
		/// \return Enum. AL_PLAYING, AL_STOPPED, etc...
		///
		ALint get_state() noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return ALuint handle integer.
		///
		const ALuint handle() const noexcept;

		///
		/// \brief Destroy all memory and OpenAL data.
		///
		/// Calls stop().
		///
		void destroy_source() noexcept;

	private:
		///
		/// Handle to source.
		///
		ALuint m_source;
	};
} // namespace frb

#endif