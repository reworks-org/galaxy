///
/// Context.hpp
/// context
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
	class Context
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
		/// Can throw exceptions.
		/// Should be in a try-catch block.
		///
		/// \return True if successful.
		///
		void initialize();

		///
		/// Optional. Change doppler factor.
		///
		/// \param factor New factor for doppler effect.
		///
		void setDopplerFactor(const float factor) noexcept;

		///
		/// Optional. Change the default speed of sound.
		///
		/// \param speed New speed of sound to define.
		///
		void setSpeedOfSound(const float speed) noexcept;

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