///
/// Audible.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_AUDIBLE_HPP_
#define FRB_AUDIBLE_HPP_

#include "frb/detail/Source.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// \brief Represents a "Sound Effect" or "Music" file.
	///
	/// Contains a Buffer and Source.
	///
	class Audible final
	{
	public:
		///
		/// Constructor.
		///
		Audible() noexcept;

		///
		/// Destructor.
		///
		~Audible() noexcept;

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
		/// Get audible buffer.
		///
		/// \return Reference to internal buffer.
		///
		frb::Buffer& buffer() noexcept;

		///
		/// Get audible source.
		///
		/// \return Reference to internal source.
		///
		frb::Source& source() noexcept;

	private:
		///
		/// Audio data buffer.
		///
		frb::Buffer m_buffer;

		///
		/// Audio in-world source.
		///
		frb::Source m_source;
	};
}

#endif