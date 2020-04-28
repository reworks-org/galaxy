///
/// Buffer.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFER_HPP_
#define FRB_BUFFER_HPP_

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

///
/// Core namespace.
///
namespace frb
{
	///
	/// Holds a single piece of audio.
	///
	class Buffer final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Generates Buffer buffer(s).
		///
		Buffer() noexcept;

		///
		/// \brief Default destructor.
		///
		/// Destroys Buffer buffer(s).
		///
		~Buffer() noexcept;

		///
		/// Load a file from disk.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		/// \return False if load failed.
		///
		bool load(const std::string& file) noexcept;

		///
		/// Load a file from memory.
		///
		/// Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param mem Unsigned char buffer containing data.
		/// \param size Size of the buffer.
		///
		/// \return False if load failed.
		///
		bool load(const unsigned char* mem, const int size) noexcept;

		///
		/// Get frequency of buffer.
		///
		/// \return Frequency as integer.
		///
		const ALint getFrequency() noexcept;

		///
		/// Get bits of buffer.
		///
		/// \return Bits as integer.
		///
		const ALint getBits() noexcept;

		///
		/// Get channels of buffer.
		///
		/// \return Channels as integer.
		///
		const ALint getChannels() noexcept;

		///
		/// Get size of buffer.
		///
		/// \return Size as integer.
		///
		const ALint getSize() noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		const ALuint handle() const noexcept;

		///
		/// Destroy all memory and OpenAL data.
		///
		void destroy() noexcept;

	private:
		///
		/// Handle to Buffer.
		///
		ALuint m_buffer;
	};
}

#endif