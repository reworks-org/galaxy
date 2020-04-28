///
/// Context.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_CONTEXT_HPP_
#define FRB_CONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>

///
/// Core namespace.
///
namespace frb
{
	///
	/// OpenAL context, device and listener.
	///
	class Context final
	{
	public:
		///
		/// Constructor.
		///
		Context() noexcept;

		///
		/// Destructor.
		///
		~Context() noexcept;

		///
		/// \brief Initialize OpenAL.
		///
		/// \return True if successful.
		///
		void initialize() noexcept;

		///
		/// \brief Change doppler factor. Optional.
		///
		/// \param factor New factor for doppler effect.
		///
		void setDopplerFactor(const float factor) noexcept;

		///
		/// \brief Change the default speed of sound. Optional.
		///
		/// \param speed New speed of sound to define.
		///
		void setSpeedOfSound(const float speed) noexcept;

		///
		/// \brief Set the gain for the listener.
		///
		/// \param gain Master gain. Must be positive.
		///
		void setListenerGain(const float gain) noexcept;

		///
		/// \brief Set the location of the listener in the world coord system.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setListenerPosition(const float x, const float y, const float z) noexcept;

		///
		/// \brief Set the audio velocity (speed and direction) of the listener.
		///
		/// \param x X velocity.
		/// \param y Y velocity.
		/// \param z Z velocity.
		///
		void setListenerVelocity(const float x, const float y, const float z) noexcept;

		///
		/// \brief Set orientation of listener.
		///
		/// \param atX "at" vector x.
		/// \param atY "at" vector y.
		/// \param atZ "at" vector z.
		/// \param upX "up" vector x.
		/// \param upY "up" vector y.
		/// \param upZ "up" vector z.
		///
		void setListenerOrientation(const float atX, const float atY, const float atZ, const float upX, const float upY, const float upZ) noexcept;

	private:
		///
		/// OpenAL device.
		///
		ALCdevice* m_device;

		///
		/// OpenAL context.
		///
		ALCcontext* m_context;
	};
}

#endif