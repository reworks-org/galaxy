///
/// Playlist.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_PLAYLIST_HPP_
#define FRB_PLAYLIST_HPP_

#include "frb/detail/Source.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// \brief Queued sound effects / music.
	///
	/// A source with queued buffers.
	///
	template<size_t length>
	class Playlist
	{
	public:
		///
		/// Constructor.
		///
		Playlist() noexcept;

		///
		/// Destructor.
		///
		~Playlist() noexcept;
		
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
		/// \brief Should the source repeat upon reaching the end.
		///
		/// \param True to repeat.
		///
		void setLooping(bool looping) noexcept;

		///
		/// Play source.
		///
		void play() noexcept;

		///
		/// Pause source.
		///
		void pause() noexcept;

		///
		/// Stop source.
		///
		void stop() noexcept;

		///
		/// Rewind source to beginning.
		///
		void rewind() noexcept;

		///
		/// Get audible source.
		///
		/// \return Reference to internal source.
		///
		frb::Source& source() noexcept;

	private:
		///
		/// Array of audio data buffers.
		///
		frb::BufferArray<length> m_buffers;

		///
		/// Audio in-world source.
		///
		frb::Source m_source;
	};

	template<size_t length>
	inline Playlist<length>::Playlist() noexcept
		:m_source()
	{
	}

	template<size_t length>
	inline Playlist<length>::~Playlist() noexcept
	{
		m_source.destroy();
		m_buffers.destroy();
	}

	template<size_t length>
	inline bool Playlist<length>::load(const std::vector<std::string>& files) noexcept
	{
		bool result = true;
		result = m_buffers.loadFromFile(files);
		
		if (result)
		{
			m_source.queue(m_buffers.raw().data(), m_buffers.raw().size());
		}

		return result;
	}

	template<size_t length>
	inline bool Playlist<length>::load(const std::vector<std::pair<unsigned char*, const int>>& data) noexcept
	{
		bool result = true;
		result = m_buffers.loadFromMemory(data);

		if (result)
		{
			m_source.queue(m_buffers.raw().data(), m_buffers.raw().size());
		}

		return result;
	}

	template<size_t length>
	inline void Playlist<length>::setLooping(bool looping) noexcept
	{
		m_source.setLooping(looping);
	}

	template<size_t length>
	inline void Playlist<length>::play() noexcept
	{
		m_source.play();
	}

	template<size_t length>
	inline void Playlist<length>::pause() noexcept
	{
		m_source.pause();
	}

	template<size_t length>
	inline void Playlist<length>::stop() noexcept
	{
		m_source.stop();
	}

	template<size_t length>
	inline void Playlist<length>::rewind() noexcept
	{
		m_source.rewind();
	}

	template<size_t length>
	inline frb::Source& Playlist<length>::source() noexcept
	{
		return m_source;
	}
}

#endif