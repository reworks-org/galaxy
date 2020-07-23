///
/// Buffer.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFER_HPP_
#define FRB_BUFFER_HPP_

#include <string_view>

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
	class Buffer
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Generates Buffer buffer(s).
		///
		Buffer();

		///
		/// \brief Default destructor.
		///
		/// Destroys Buffer buffer(s).
		///
		virtual ~Buffer() noexcept;

		///
		/// Get frequency of buffer.
		///
		/// \return Frequency as integer.
		///
		const ALint get_frequency() const noexcept;

		///
		/// Get channels of buffer.
		///
		/// \return Channels as integer.
		///
		const ALint get_channels() const noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		const ALuint handle() const noexcept;

	protected:
		///
		/// Load a file from disk.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		/// \return False if load failed.
		///
		bool internal_load(std::string_view file);

		///
		/// Destroy all memory and OpenAL data.
		///
		void destroy_buffer() noexcept;

		///
		/// Handle to Buffer.
		///
		ALuint m_buffer;

		///
		/// Amount of channels in buffer.
		///
		ALint m_channels;

		///
		/// Frequency of audio.
		///
		ALsizei m_frequency;
	};
} // namespace frb

#endif