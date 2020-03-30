///
/// BufferArray.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFERARRAY_HPP_
#define FRB_BUFFERARRAY_HPP_

#include <stdexcept>
#include <filesystem>

#include <AL/al.h>
#include <AL/alc.h>
#include <stb_vorbis.h>

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
		/// Can throw exceptions.
		///
		BufferArray();

		///
		/// \brief Default destructor.
		///
		/// Destroys BufferArray BufferArray(s).
		///
		~BufferArray() noexcept;

		///
		/// Load files from disk.
		///
		/// Can throw exceptions.
		///
		/// \param files Array of files to load from disk. Can only load ogg vorbis.
		///
		void load(const std::vector<std::string>& files);

		///
		/// Load files from memory.
		///
		/// Can throw exceptions. Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param data A set of paired mem/size files to load from memory.
		///
		void load(const std::vector<std::pair<unsigned char*, const int>>& data);

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
		/// Can throw exceptions.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		/// \param buffer OpenAL buffer index.
		///
		void iloadFile(const std::string& file, const ALuint buffer);

		///
		/// Load a file from memory.
		///
		/// Can throw exceptions. Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param mem Unsigned char buffer containing data.
		/// \param size Size of the buffer.
		/// \param buffer OpenAL buffer index.
		///
		void iloadMem(const unsigned char* mem, const int size, const ALuint buffer);

	private:
		///
		/// Handle to BufferArray.
		///
		std::array<ALuint, size> m_bufferArray;
	};

	template<size_t size>
	inline BufferArray<size>::BufferArray()
	{
		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(size, m_bufferArray.data());
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to gen audio buffer."));
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
	inline void BufferArray<size>::load(const std::vector<std::string>& files)
	{
		for (ALuint count = 0; count < size; count++)
		{
			iloadFile(files[count], count);
		}
	}

	template<size_t size>
	inline void BufferArray<size>::load(const std::vector<std::pair<unsigned char*, const int>>& data)
	{
		for (ALuint count = 0; count < size; count++)
		{
			iloadMem(data[count].first, data[count].second, count);
		}
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
	inline void BufferArray<size>::iloadFile(const std::string& file, const ALuint buffer)
	{
		// Process filepath properly.
		auto path = std::filesystem::path(file);
		if (path.extension() != ".ogg")
		{
			throw std::runtime_error("File must be ogg vorbis and have extension of .ogg!");
		}
		else
		{
			int channels = 0;
			int samples = 0;
			short* data = nullptr;

			auto length = stb_vorbis_decode_filename(path.string().c_str(), &channels, &samples, &data);
			if (length < 1)
			{
				// Make sure data is freed.
				if (data != nullptr)
				{
					free(data);
				}

				if (length == -1)
				{
					throw std::runtime_error("Failed to open file with stb_vorbis.");
				}
				else if (length == -2)
				{
					throw std::runtime_error("Failed to parse with stb_vorbis.");
				}
				else
				{
					throw std::runtime_error("Failed due to unknown error. Error code returned: " + std::to_string(length));
				}
			}
			else
			{
				auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
				alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

				free(data);
			}
		}
	}

	template<size_t size>
	inline void BufferArray<size>::iloadMem(const unsigned char* mem, const int size, const ALuint buffer)
	{
		int channels = 0;
		int samples = 0;
		short* data = nullptr;

		auto length = stb_vorbis_decode_memory(mem, size, &channels, &samples, &data);
		if (length < 1)
		{
			// Make sure data is freed.
			if (data != nullptr)
			{
				free(data);
			}

			if (length == -1)
			{
				throw std::runtime_error("Failed to open file with stb_vorbis.");
			}
			else if (length == -2)
			{
				throw std::runtime_error("Failed to parse with stb_vorbis.");
			}
			else
			{
				throw std::runtime_error("Failed due to unknown error. Error code returned: " + std::to_string(length));
			}
		}
		else
		{
			auto format = (channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			alBufferData(buffer, format, data, channels * length * sizeof(short), samples);

			free(data);
		}
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getFrequency() noexcept
	{
		ALint freq = 0;
		alGetBufferi(m_bufferArray[index], AL_FREQUENCY, &freq);

		return std::move(freq);
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getBits() noexcept
	{
		ALint bits = 0;
		alGetBufferi(m_bufferArray[index], AL_BITS, &bits);

		return std::move(bits);
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getChannels() noexcept
	{
		ALint channels = 0;
		alGetBufferi(m_bufferArray[index], AL_CHANNELS, &channels);

		return std::move(channels);
	}

	template<size_t size>
	template<size_t index>
	inline const ALint BufferArray<size>::getSize() noexcept
	{
		ALint l_size = 0;
		alGetBufferi(m_bufferArray[index], AL_SIZE, &l_size);

		return std::move(l_size);
	}

	template<size_t size>
	template<size_t index>
	inline const ALuint BufferArray<size>::handle() const noexcept
	{
		return m_bufferArray[index];
	}
}

#endif