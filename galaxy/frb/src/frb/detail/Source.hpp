///
/// Source.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCE_HPP_
#define FRB_SOURCE_HPP_

#include <vector>

#include <protostar/system/Concepts.hpp>

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
		/// \param buffer Pointer to an frb::Buffer* object.
		///
		void queue(pr::not_nullptr auto buffer);

		///
		/// Queue a streamed buffer for the source to play through.
		///
		/// \param stream_buffer Pointer to a BufferStream* object.
		///
		void queue(pr::not_nullptr auto stream_buffer);

		///
		/// Queue a set of buffers for the source to play through.
		///
		/// \param buffers Array of buffers you want to play in a queue.
		///
		void queue(const std::vector<frb::Buffer>& buffers);

		///
		/// Raw queue of buffer(s) to play.
		///
		/// \param buffer_array Array of buffers you want to play in a queue. Cannot be nullptr.
		/// \param size The size/length of buffer_array.
		///
		void queue(const ALuint* buffer_array, const pr::positive_size_t auto size);

		///
		/// \brief Get current state of the source.
		///
		/// \return Enum. AL_PLAYING, AL_STOPPED, etc...
		///
		[[nodisard]] ALint get_state() noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return ALuint handle integer.
		///
		[[nodisard]] const ALuint handle() const noexcept;

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