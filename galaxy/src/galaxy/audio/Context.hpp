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
#include <glm/vec3.hpp>

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
			/// \param pos Position in world.
			///
			void set_listener_position(const glm::vec3& pos) noexcept;

			///
			/// Set the audio velocity (speed and direction) of the listener.
			///
			/// \param vel Velocity vector.
			///
			void set_listener_velocity(const glm::vec3& vel) noexcept;

			///
			/// Set orientation of listener.
			///
			/// \param at "at" vector.
			/// \param up "up" vector.
			///
			void set_listener_orientation(const glm::vec3& at, const glm::vec3& up) noexcept;

		private:
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