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
		/// Copy constructor.
		///
		Buffer(const Buffer&) noexcept = delete;

		///
		/// Move constructor.
		///
		Buffer(Buffer&&);

		///
		/// Copy assignment operator.
		///
		Buffer& operator=(const Buffer&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Buffer& operator=(Buffer&&);

		///
		/// \brief Default destructor.
		///
		/// Destroys Buffer buffer(s).
		///
		virtual ~Buffer();

		///
		/// Get frequency of buffer.
		///
		/// \return Frequency as integer.
		///
		[[nodiscard]] const ALint get_frequency() const noexcept;

		///
		/// Get channels of buffer.
		///
		/// \return Channels as integer.
		///
		[[nodiscard]] const ALint get_channels() const noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		[[nodiscard]] const ALuint handle() const noexcept;

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