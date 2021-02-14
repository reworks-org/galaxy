///
/// SourceManipulator.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_SOURCEMANIPULATOR_HPP_
#define GALAXY_AUDIO_SOURCEMANIPULATOR_HPP_

#include <AL/al.h>
#include <AL/alc.h>
#include <glm/vec3.hpp>

#include "galaxy/audio/Source.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// Origin of the audio sound.
		///
		class SourceManipulator
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~SourceManipulator() = default;

			///
			/// \brief Play source.
			///
			/// Plays source from beginning or pause point.
			///
			virtual void play() = 0;

			///
			/// Pause source.
			///
			virtual void pause() = 0;

			///
			/// \brief Stop source.
			///
			/// Starts again from beginning.
			///
			virtual void stop() = 0;

			///
			/// Audio pitch.
			///
			/// \param pitch Multiplier value. I.e. 0.5f is a *0.5 multiplier.
			///
			void set_pitch(const float pitch);

			///
			/// Audio gain.
			///
			/// \param gain Multiplier value. I.e. 0.5f is a *0.5 multiplier.
			///
			void set_gain(const float gain);

			///
			/// \brief Set RollOff factor for source.
			///
			/// \param factor Floating point factor value.
			///
			void set_rolloff_factor(const float factor);

			///
			/// \brief Set maximum distance from which there is no attenuation afterwards.
			///
			/// \param distance Floating point distance value.
			///
			void set_max_distance(const float distance);

			///
			/// \brief Configure audio cone.
			///
			/// \param outer_gain The gain when outside the oriented cone.
			/// \param inner_angle The gain when inside the oriented cone.
			/// \param outer_angle Outer angle of the sound cone, in degrees. Defaults to 360.
			///
			void set_cone(const float outer_gain, const float inner_angle, const float outer_angle = 360.0f);

			///
			/// \brief Specifies the current location of the object in the world coordinate system.
			///
			/// \param pos Position in world.
			///
			void set_position(const glm::vec3& pos);

			///
			/// \brief Specifies the current velocity (speed and direction) of the object, in theworld coordinate system
			///
			/// \param vel Velocity of sound.
			///
			void set_velocity(const glm::vec3& vel);

			///
			/// \brief Source is directional. The sound emission is presumed to be symmetric around the direction vector.
			///
			/// \param dir Direction of sound.
			///
			void set_direction(const glm::vec3& dir);

			///
			/// \brief Should the source repeat upon reaching the end.
			///
			/// \param looping True to repeat.
			///
			virtual void set_looping(const bool looping) = 0;

			///
			/// Retrieve OpenAL source state.
			///
			/// \return Integer of enum AL_SOURCE_STATE.
			///
			[[nodiscard]] const ALint get_state();

		protected:
			///
			/// Default constructor.
			///
			SourceManipulator() noexcept = default;

		protected:
			///
			/// OpenAL audio source object.
			///
			Source m_source;

		private:
			///
			/// Move constructor.
			///
			SourceManipulator(SourceManipulator&&) = delete;

			///
			/// Move assignment operator.
			///
			SourceManipulator& operator=(SourceManipulator&&) = delete;

			///
			/// Copy constructor.
			///
			SourceManipulator(const SourceManipulator&) = delete;

			///
			/// Copy assignment operator.
			///
			SourceManipulator& operator=(const SourceManipulator&) = delete;
		};
	} // namespace audio
} // namespace galaxy

#endif