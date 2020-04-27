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
		Audible();

		///
		/// Destructor.
		///
		~Audible() noexcept;

		///
		/// Load a file from disk.
		///
		/// Can throw exceptions.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		void load(const std::string& file);

		///
		/// Load a file from memory.
		///
		/// Can throw exceptions. Memory is NOT freed. You MUST free *mem yourself.
		///
		/// \param mem Unsigned char buffer containing data.
		/// \param size Size of the buffer.
		///
		void load(const unsigned char* mem, const int size);

		///
		/// \brief Should the source repeat upon reaching the end.
		///
		/// Can throw exceptions.
		///
		/// \param True to repeat.
		///
		void setLooping(bool looping);

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