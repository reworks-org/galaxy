///
/// Buffer.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFER_HPP_
#define FRB_BUFFER_HPP_

#include <AL/al.h>
#include <AL/alc.h>

///
/// Core namespace.
///
namespace frb
{
	///
	/// Origin of the audio sound.
	///
	class Buffer
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Can throw exceptions.
		/// Generates Buffer buffer(s).
		///
		Buffer();

		///
		/// \brief Default destructor.
		///
		/// Destroys Buffer buffer(s).
		///
		~Buffer() noexcept;

		///
		///
		///
		void load(const std::string& file);

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		const ALuint handle() const noexcept;

	private:
		///
		/// Handle to Buffer.
		///
		ALuint m_buffer;
	};
}

#endif