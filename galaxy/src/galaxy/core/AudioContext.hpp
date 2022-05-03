///
/// AudioContext.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_AUDIOCONTEXT_HPP_
#define GALAXY_CORE_AUDIOCONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>
#include <glm/vec3.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// OpenAL context, device and listener.
		///
		class AudioContext final
		{
		public:
			///
			/// Distance model enum.
			///
			enum class DistanceModel : ALenum
			{
				///
				/// gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE
				///		+ AL_ROLLOFF_FACTOR * (distance - AL_REFERENCE_DISTANCE));
				///
				INVERSE_DISTANCE = AL_INVERSE_DISTANCE,

				///
				/// distance = max(distance, AL_REFERENCE_DISTANCE);
				///	distance = min(distance, AL_MAX_DISTANCE);
				///	gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR
				///		* (distance - AL_REFERENCE_DISTANCE));
				///
				INVERSE_DISTANCE_CLAMPED = AL_INVERSE_DISTANCE_CLAMPED,

				///
				/// distance = min(distance, AL_MAX_DISTANCE);	// Avoid negative gain
				///	gain = (1 - AL_ROLLOFF_FACTOR * (distance - AL_REFERENCE_DISTANCE)
				///		/ (AL_MAX_DISTANCE - AL_REFERENCE_DISTANCE));
				///
				LINEAR_DISTANCE = AL_LINEAR_DISTANCE,

				///
				/// distance = max(distance, AL_REFERENCE_DISTANCE);
				///	distance = min(distance, AL_MAX_DISTANCE);
				///	gain = (1 - AL_ROLLOFF_FACTOR * (distance - AL_REFERENCE_DISTANCE)
				///		/ (AL_MAX_DISTANCE - AL_REFERENCE_DISTANCE));
				///
				LINEAR_DISTANCE_CLAMPED = AL_LINEAR_DISTANCE_CLAMPED,

				///
				/// gain = pow(distance / AL_REFERENCE_DISTANCE, -AL_ROLLOFF_FACTOR);
				///
				EXPONENT_DISTANCE = AL_EXPONENT_DISTANCE,

				///
				/// distance = max(distance, AL_REFERENCE_DISTANCE);
				/// distance = min(distance, AL_MAX_DISTANCE);
				/// gain = pow(distance / AL_REFERENCE_DISTANCE, -AL_ROLLOFF_FACTOR);
				///
				EXPONENT_DISTANCE_CLAMPED = AL_EXPONENT_DISTANCE_CLAMPED,

				///
				/// gain = 1;
				///
				NONE = AL_NONE
			};

			///
			/// Constructor.
			///
			AudioContext();

			///
			/// Destructor.
			///
			~AudioContext();

			///
			/// \brief Suspend context processing.
			///
			/// When a context is suspended, changes in OpenAL state will be accepted but will not be processed.
			/// A typical use of alcSuspendContext would be to suspend a context, apply all the OpenAL state
			/// changes at once, and then call process() to apply all the state changes at once.
			///
			void suspend() noexcept;

			///
			/// Resume context processing.
			///
			void resume() noexcept;

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
			/// Set the OpenAL Distance Model.
			///
			/// \param model Enum model to use.
			///
			void set_distance_model(const DistanceModel model) noexcept;

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

			///
			/// Check if an OpenAL extension is present.
			///
			/// \param name Extension name.
			///
			/// \return True if extension is present.
			///
			[[nodiscard]] bool has_extension(const ALchar* name) noexcept;

			///
			/// Check if an OpenAL contextg extension is present.
			///
			/// \param name Context extension name.
			///
			/// \return True if extension is present.
			///
			[[nodiscard]] bool has_context_extension(const ALchar* name) noexcept;

			///
			/// Get doppler factor.
			///
			/// \return Global doppler factor as float.
			///
			[[nodiscard]] float get_dopper_factor() noexcept;

			///
			/// Get speed of sound.
			///
			/// \return Used speed of sound as a float.
			///
			[[nodiscard]] float get_speed_of_sound() noexcept;

			///
			/// Get distance model.
			///
			/// \return Global distance model as a float.
			///
			[[nodiscard]] float get_distance_model() noexcept;

			///
			/// Get the gain for the listener.
			///
			/// \return Returns a const float.
			///
			[[nodiscard]] float get_listener_gain() noexcept;

			///
			/// Get the location of the listener in the world coord system.
			///
			/// \return Vec3 containing position.
			///
			[[nodiscard]] glm::vec3 get_listener_position() noexcept;

			///
			/// Get the audio velocity (speed and direction) of the listener.
			///
			/// \return Vec3 containing velocity.
			///
			[[nodiscard]] glm::vec3 get_listener_velocity() noexcept;

		private:
			///
			/// Copy constructor.
			///
			AudioContext(const AudioContext&) = delete;

			///
			/// Move constructor.
			///
			AudioContext(AudioContext&&) = delete;

			///
			/// Copy assignment operator.
			///
			AudioContext& operator=(const AudioContext&) = delete;

			///
			/// Move assignment operator.
			///
			AudioContext& operator=(AudioContext&&) = delete;

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
	} // namespace core
} // namespace galaxy

#endif