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
	template<size_t size>
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
		/// \return Raw std::array of handles.
		///
		std::array<ALuint, size>& raw() noexcept;

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
		std::array<ALuint, size> m_bufferArray;
	};

	template<size_t size>
	inline BufferArray<size>::BufferArray() noexcept
	{
		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(size, m_bufferArray.data());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to gen audio buffer."));
		}
	}

	template<size_t size>
	inline BufferArray<size>::~BufferArray() noexcept
	{
		for (auto count = 0; count < size; count++)
		{
			if (m_bufferArray[count] != static_cast<ALuint>((int)-1))
			{
				alDeleteBuffers(1, &m_bufferArray[count]);
			}
		}
	}

	template<size_t size>
	inline bool BufferArray<size>::load(const std::vector<std::string>& files) noexcept
	{
		bool result = true;

		for (ALuint count = 0; count < size; count++)
		{
			result = iloadFile(files[count], count);
		}

		return result;
	}

	template<size_t size>
	inline bool BufferArray<size>::load(const std::vector<std::pair<unsigned char*, const int>>& data) noexcept
	{
		bool result = true;

		for (ALuint count = 0; count < size; count++)
		{
			result = iloadMem(data[count].first, data[count].second, count);
		}

		return result;
	}

	template<size_t size>
	inline std::array<ALuint, size>& BufferArray<size>::raw() noexcept
	{
		return m_bufferArray;
	}

	template<size_t size>
	inline void BufferArray<size>::destroy() noexcept
	{
		alDeleteBuffers(size, m_bufferArray.data());
		for (auto count = 0; count < size; count++)
		{
			m_bufferArray[count] = static_cast<ALuint>((int)-1);
		}
	}

	template<size_t size>
	inline bool BufferArray<size>::iloadFile(const std::string& file, const ALuint buffer) noexcept
	{
		bool result = true;

		// Process filepath properly.
		auto path = std::filesystem::path(file);
		if (path.extension() != ".ogg")
		{
			PL_LOG(PL_ERROR, "File must be ogg vorbis and have extension of .ogg!");
			result = false;
		}
		else
		{
			int channels = 0;
			int samples = 0;
			short* data = nullptr;

			auto length = stb_vorbis_decode_filename(path.string().c_str(), &channels, &samples, &data);
			if (length < 1)
			{
				result = false;

				// Make sure data is freed.
				if (data != nullptr)
				{
					free(data);
				}

				if (length == -1)
				{
					PL_LOG(PL_ERROR, "Failed to open file with stb_vorbis.");
				}
				else if (length == -2)
				{
					PL_LOG(PL_ERROR, "Failed to parse with stb_vorbis.");
				}
				else
				{
					PL_LOG(PL_ERROR, "Failed due to unknown error. Error code returned: " + std::to_string(length));
				}
			}
			else
			{
				auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
				alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

				free(data);
			}
		}

		return result;
	}

	template<size_t size>
	inline bool BufferArray<size>::iloadMem(const unsigned char* mem, const int size, const ALuint buffer) noexcept
	{
		int channels = 0;
		int samples = 0;
		short* data = nullptr;
		bool result = true;

		auto length = stb_vorbis_decode_memory(mem, size, &channels, &samples, &data);
		if (length < 1)
		{
			result = false;

			// Make sure data is freed.
			if (data != nullptr)
			{
				free(data);
			}

			if (length == -1)
			{
				PL_LOG(PL_ERROR, "Failed to open file with stb_vorbis.");
			}
			else if (length == -2)
			{
				PL_LOG(PL_ERROR, "Failed to parse with stb_vorbis.");
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed due to unknown error. Error code returned: " + std::to_string(length));
			}
		}
		else
		{
			auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

			free(data);
		}

		return result;
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getFrequency() noexcept
	{
		ALint freq = 0;
		alGetBufferi(m_bufferArray[index], AL_FREQUENCY, &freq);

		return freq;
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getBits() noexcept
	{
		ALint bits = 0;
		alGetBufferi(m_bufferArray[index], AL_BITS, &bits);

		return bits;
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getChannels() noexcept
	{
		ALint channels = 0;
		alGetBufferi(m_bufferArray[index], AL_CHANNELS, &channels);

		return channels;
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getSize() noexcept
	{
		ALint l_size = 0;
		alGetBufferi(m_bufferArray[index], AL_SIZE, &l_size);

		return l_size;
	}

	template<size_t size>
	template<size_t index>
	inline const ALuint BufferArray<size>::handle() const noexcept
	{
		return m_bufferArray[index];
	}
}

#endif