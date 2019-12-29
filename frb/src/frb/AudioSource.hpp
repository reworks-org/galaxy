///
/// AudioSource.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_AUDIOSOURCE_HPP_
#define FRB_AUDIOSOURCE_HPP_

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
	class AudioSource
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Can throw exceptions.
		/// Generates source buffer(s).
		///
		AudioSource();

		///
		/// \brief Default destructor.
		///
		/// Destroys source buffer(s).
		///
		~AudioSource() noexcept;

		///
		/// \brief
		///
		/// Can throw exceptions.
		///
		/// \param
		///
		void setPitch(const float pitch);

		///
		/// \brief
		///
		/// Can throw exceptions.
		///
		/// \param
		///
		void setGain(const float gain);

		///
		/// \brief
		///
		/// Can throw exceptions.
		///
		/// \param
		/// \param
		/// \param
		///
		void setPosition(const float x, const float y, const float z);
		
		///
		/// \brief
		///
		/// Can throw exceptions.
		///
		/// \param
		/// \param
		/// \param
		///
		void setVelocity(const float x, const float y, const float z);

		///
		/// \brief
		///
		/// Can throw exceptions.
		///
		/// \param
		///
		void setLooping(bool looping);

	private:
		///
		/// Handle to source.
		///
		ALuint m_source;
	};
}

#endif