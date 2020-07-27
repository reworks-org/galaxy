///
/// SourceManipulator.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCEMANIPULATOR_HPP_
#define FRB_SOURCEMANIPULATOR_HPP_

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Origin of the audio sound.
	///
	class SourceManipulator
	{
	public:
		///
		/// Virtual default destructor.
		///
		virtual ~SourceManipulator() noexcept = default;

		///
		/// \brief Play source.
		///
		/// Plays source from beginning or pause point.
		///
		void play() noexcept;

		///
		/// Pause source.
		///
		void pause() noexcept;

		///
		/// \brief Stop source.
		///
		/// Starts again from beginning when play() or resume() are called.
		///
		void stop() noexcept;

		///
		/// Rewind source to beginning.
		///
		void rewind() noexcept;

		///
		/// Audio pitch.
		///
		/// \param pitch Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void set_pitch(pr::positive_float auto pitch) noexcept;

		///
		/// Audio gain.
		///
		/// \param gain Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void set_gain(pr::positive_float auto gain) noexcept;

		///
		/// \brief Set RollOff factor for source.
		///
		/// \param factor Floating point factor value.
		///
		void set_rolloff_factor(pr::positive_float auto factor) noexcept;

		///
		/// \brief Set maximum distance from which there is no attenuation afterwards.
		///
		/// \param distance Floating point distance value.
		///
		void set_max_distance(const float distance) noexcept;

		///
		/// \brief Configure audio cone.
		///
		/// \param outerGain The gain when outside the oriented cone.
		/// \param innerAngle The gain when inside the oriented cone.
		/// \param outerAngle Outer angle of the sound cone, in degrees. Default is 360.
		///
		void set_cone(const float outer_gain, const float inner_angle, const float outer_angle = 360.0f) noexcept;

		///
		/// \brief Specifies the current location of the object in the world coordinate system.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void set_position(const float x, const float y, const float z) noexcept;

		///
		/// \brief Specifies the current velocity (speed and direction) of the object, in theworld coordinate system
		///
		/// \param x X velocity.
		/// \param y Y velocity.
		/// \param z Z velocity.
		///
		void set_velocity(const float x, const float y, const float z) noexcept;

		///
		/// \brief Source is directional. The sound emission is presumed to be symmetric around the direction vector.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void set_direction(const float x, const float y, const float z) noexcept;

		///
		/// \brief Should the source repeat upon reaching the end.
		///
		/// \param True to repeat.
		///
		void set_looping(bool looping) noexcept;

	protected:
		///
		/// Deleted default constructor.
		///
		SourceManipulator() noexcept;

		///
		/// Set source to manipulate.
		///
		/// \param source Pointer to source handle in OpenAL.
		///
		void set_source_to_manipulate(const pr::positive_uint auto source) noexcept;

		///
		/// Pointer to source being used internally.
		///
		unsigned int m_source;
	};
} // namespace frb

#endif