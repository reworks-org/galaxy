///
/// Context.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_CONTEXT_HPP_
#define GALAXY_AUDIO_CONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace audio
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
			Context();

			///
			/// Copy constructor.
			///
			Context(const Context&) = delete;

			///
			/// Move constructor.
			///
			Context(Context&&) = delete;

			///
			/// Copy assignment operator.
			///
			Context& operator=(const Context&) = delete;

			///
			/// Move assignment operator.
			///
			Context& operator=(Context&&) = delete;

			///
			/// Destructor.
			///
			~Context();

			///
			/// Change doppler factor.
			///
			/// \param factor New factor for doppler effect. Multiplicative.
			///
			void set_doppler_factor(const float factor);

			///
			/// Change the default speed of sound. Optional.
			///
			/// \param speed New speed of sound to define. Multiplicative.
			///
			void set_speed_of_sound(const float speed);

			///
			/// Set the gain for the listener.
			///
			/// \param gain Master gain. Must be positive. Multiplicative.
			///
			void set_listener_gain(const float gain);

			///
			/// Set the location of the listener in the world coord system.
			///
			/// \param x position in world.
			/// \param y position in world.
			/// \param z position in world.
			///
			void set_listener_position(const float x, const float y, const float z);

			///
			/// Set the audio velocity (speed and direction) of the listener.
			///
			/// \param x X velocity.
			/// \param y Y velocity.
			/// \param z Z velocity.
			///
			void set_listener_velocity(const float x, const float y, const float z);

			///
			/// Set orientation of listener.
			///
			/// \param atX "at" vector x.
			/// \param atY "at" vector y.
			/// \param atZ "at" vector z.
			/// \param upX "up" vector x.
			/// \param upY "up" vector y.
			/// \param upZ "up" vector z.
			///
			void set_listener_orientation(const float atX, const float atY, const float atZ, const float upX, const float upY, const float upZ);

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
	} // namespace audio
} // namespace galaxy

#endif