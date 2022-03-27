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
			/// Destructor.
			///
			virtual ~SourceManipulator() = default;

			///
			/// \brief Play source.
			///
			/// Plays source from beginning or pause point.
			///
			virtual void play() noexcept = 0;

			///
			/// Pause source.
			///
			virtual void pause() noexcept = 0;

			///
			/// \brief Stop source.
			///
			/// Starts again from beginning.
			///
			virtual void stop() noexcept = 0;

			///
			/// Audio pitch.
			///
			/// \param pitch Multiplier value. I.e. 0.5f is a *0.5 multiplier. 0.0f - 10.0f.
			///
			void set_pitch(const float pitch) noexcept;

			///
			/// Audio gain.
			///
			/// \param gain Multiplier value. I.e. 0.5f is a *0.5 multiplier. 0.0f - 10.0f.
			///
			void set_gain(const float gain) noexcept;

			///
			/// \brief Set RollOff factor for source.
			///
			/// \param factor Floating point factor value. 0.0f - 10.0f.
			///
			void set_rolloff_factor(const float factor) noexcept;

			///
			/// \brief Set maximum distance from which there is no attenuation afterwards.
			///
			/// \param distance Floating point distance value.
			///
			void set_max_distance(const float distance) noexcept;

			///
			/// \brief Configure audio cone.
			///
			/// \param outer_gain The gain when outside the oriented cone.
			/// \param inner_gain The gain when inside the oriented cone.
			/// \param outer_angle Outer angle of the sound cone, in degrees. Defaults to 360.
			///
			void set_cone(const float outer_gain, const float inner_gain, const float outer_angle = 360.0f) noexcept;

			///
			/// \brief Specifies the current location of the object in the world coordinate system.
			///
			/// \param pos Position in world.
			///
			void set_position(const glm::vec3& pos) noexcept;

			///
			/// \brief Specifies the current velocity (speed and direction) of the object, in theworld coordinate system
			///
			/// \param vel Velocity of sound.
			///
			void set_velocity(const glm::vec3& vel) noexcept;

			///
			/// \brief Source is directional. The sound emission is presumed to be symmetric around the direction vector.
			///
			/// \param dir Direction of sound.
			///
			void set_direction(const glm::vec3& dir) noexcept;

			///
			/// \brief Should the source repeat upon reaching the end.
			///
			/// \param looping True to repeat.
			///
			virtual void set_looping(const bool looping) noexcept = 0;

			///
			/// Retrieve OpenAL source state.
			///
			/// \return Integer of enum AL_SOURCE_STATE.
			///
			[[nodiscard]] ALint get_state() noexcept;

			///
			/// Get audio pitch.
			///
			/// \return Const float.
			///
			[[nodiscard]] float get_pitch() noexcept;

			///
			/// Get audio gain.
			///
			/// \return Const float.
			///
			[[nodiscard]] float get_gain() noexcept;

			///
			/// Get audio rolloff factor.
			///
			/// \return Const float.
			///
			[[nodiscard]] float get_rolloff_factor() noexcept;

			///
			/// Get audio max distance.
			///
			/// \return Const float.
			///
			[[nodiscard]] float get_max_distance() noexcept;

			///
			/// Get audio cone.
			///
			/// \return Vector3. x = outer_gain, y =  inner_gain, z = outer_angle.
			///
			[[nodiscard]] glm::vec3 get_cone() noexcept;

			///
			/// Get audio pitch.
			///
			/// \return Vector3.
			///
			[[nodiscard]] glm::vec3 get_position() noexcept;

			///
			/// Get audio velocity.
			///
			/// \return Vector3.
			///
			[[nodiscard]] glm::vec3 get_velocity() noexcept;

			///
			/// Get audio direction.
			///
			/// \return Vector3.
			///
			[[nodiscard]] glm::vec3 get_direction() noexcept;

			///
			/// Get audio looping state.
			///
			/// \return Const bool.
			///
			[[nodiscard]] virtual bool get_looping() noexcept = 0;

		protected:
			///
			/// Constructor.
			///
			SourceManipulator() = default;

		protected:
			///
			/// OpenAL audio source object.
			///
			Source m_source;

			///
			/// Audio filename.
			///
			std::string m_filename;

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