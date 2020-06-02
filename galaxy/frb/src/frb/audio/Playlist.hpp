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
		frb::BufferArray m_buffers;

		///
		/// Audio in-world source.
		///
		frb::Source m_source;
	};
}

#endif