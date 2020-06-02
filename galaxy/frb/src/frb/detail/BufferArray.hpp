///
/// BufferArray.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFERARRAY_HPP_
#define FRB_BUFFERARRAY_HPP_

#include <filesystem>

#include <AL/al.h>
#include <AL/alc.h>
#include <stb_vorbis.h>
#include <pulsar/Log.hpp>


#include "frb/Error.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Holds multiple audio sounds.
	///
	class BufferArray
	{
	public:
		///
		/// \brief Default constructor.
		///
		BufferArray() noexcept;

		///
		/// \brief Default destructor.
		///
		/// Destroys BufferArray BufferArray(s).
		///
		~BufferArray() noexcept;

		///
		/// Load files from disk.
		///
		/// \param files Array of files to load from disk. Can only load ogg vorbis.
		///
		/// \return False if load failed.
		///
		bool load(const std::vector<std::string>& files) noexcept;

		///
		/// Load files from memory.
		///
		/// Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param data A set of paired mem/size files to load from memory.
		///
		/// \return False if load failed.
		///
		bool load(const std::vector<std::pair<unsigned char*, const int>>& data) noexcept;

		///
		/// Get frequency of buffer.
		///
		/// \return Frequency as integer.
		///
		template<size_t index>
		const ALint getFrequency() noexcept;

		///
		/// Get bits of buffer.
		///
		/// \return Bits as integer.
		///
		template<size_t index>
		const ALint getBits() noexcept;

		///
		/// Get channels of buffer.
		///
		/// \return Channels as integer.
		///
		template<size_t index>
		const ALint getChannels() noexcept;

		///
		/// Get size of buffer.
		///
		/// \return Size as integer.
		///
		template<size_t index>
		const ALint getSize() noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		template<size_t index>
		const ALuint handle() const noexcept;

		///
		/// Retrieve internal raw array of handles.
		///
		/// \return Std::vector of handles.
		///
		std::vector<ALuint>& raw() noexcept;

		///
		/// Destroy all memory and OpenAL data.
		///
		void destroy() noexcept;

	private:
		///
		/// Load a file from disk.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		/// \param buffer OpenAL buffer index.
		///
		/// \return False if load failed.
		///
		bool iloadFile(const std::string& file, const ALuint buffer) noexcept;

		///
		/// Load a file from memory.
		///
		/// Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param mem Unsigned char buffer containing data.
		/// \param size Size of the buffer.
		/// \param buffer OpenAL buffer index.
		///
		/// \return False if load failed.
		///
		bool iloadMem(const unsigned char* mem, const int size, const ALuint buffer) noexcept;

	private:
		///
		/// Handle to BufferArray.
		///
		std::vector<ALuint> m_bufferArray;
	};

	template<size_t index>
	inline const ALint BufferArray::getFrequency() noexcept
	{
		ALint freq = 0;
		alGetBufferi(m_bufferArray[index], AL_FREQUENCY, &freq);

		return freq;
	}

	template<size_t index>
	inline const ALint BufferArray::getBits() noexcept
	{
		ALint bits = 0;
		alGetBufferi(m_bufferArray[index], AL_BITS, &bits);

		return bits;
	}

	template<size_t index>
	inline const ALint BufferArray::getChannels() noexcept
	{
		ALint channels = 0;
		alGetBufferi(m_bufferArray[index], AL_CHANNELS, &channels);

		return channels;
	}

	template<size_t index>
	inline const ALint BufferArray::getSize() noexcept
	{
		ALint l_size = 0;
		alGetBufferi(m_bufferArray[index], AL_SIZE, &l_size);

		return l_size;
	}

	template<size_t index>
	inline const ALuint BufferArray::handle() const noexcept
	{
		return m_bufferArray[index];
	}
}

#endif