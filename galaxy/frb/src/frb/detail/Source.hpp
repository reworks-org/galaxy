///
/// Source.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCE_HPP_
#define FRB_SOURCE_HPP_

#include <vector>

#include "frb/detail/Buffer.hpp"
#include "frb/detail/BufferArray.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Origin of the audio sound.
	///
	class Source final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Generates source buffer(s).
		///
		Source() noexcept;

		///
		/// \brief Default destructor.
		///
		/// Destroys source buffer(s).
		///
		~Source() noexcept;

		///
		/// \brief Audio pitch.
		///
		/// Negative values automatically converted to positive.
		///
		/// \param pitch Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void setPitch(const float pitch) noexcept;

		///
		/// \brief Audio gain.
		///
		/// Negative values automatically converted to positive.
		///
		/// \param gain Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void setGain(const float gain) noexcept;

		///
		/// \brief Set RollOff factor for source.
		///
		/// \param factor Floating point factor value.
		///
		void setRolloffFactor(const float factor) noexcept;

		///
		/// \brief Set maximum distance from which there is no attenuation afterwards.
		///
		/// \param distance Floating point distance value.
		///
		void setMaxDistance(const float distance) noexcept;

		///
		/// \brief Configure audio cone.
		///
		/// \param outerGain The gain when outside the oriented cone.
		/// \param innerAngle The gain when inside the oriented cone.
		/// \param outerAngle Outer angle of the sound cone, in degrees. Default is 360.
		///
		void configureCone(const float outerGain, const float innerAngle, const float outerAngle = 360.0f) noexcept;

		///
		/// \brief Specifies the current location of the object in the world coordinate system.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setPosition(const float x, const float y, const float z) noexcept;
		
		///
		/// \brief Specifies the current velocity (speed and direction) of the object, in theworld coordinate system
		///
		/// \param x X velocity.
		/// \param y Y velocity.
		/// \param z Z velocity.
		///
		void setVelocity(const float x, const float y, const float z) noexcept;

		///
		/// \brief Source is directional. The sound emission is presumed to be symmetric around the direction vector.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setDirection(const float x, const float y, const float z) noexcept;

		///
		/// \brief Should the source repeat upon reaching the end.
		///
		/// \param True to repeat.
		///
		void setLooping(bool looping) noexcept;

		///
		/// \brief Get current state of the source.
		///
		/// \return Enum. AL_PLAYING, AL_STOPPED, etc...
		///
		ALint getState() noexcept;

		///
		/// Bind an audio buffer to an audio source.
		///
		/// \param buffer Buffer audio to play / manipulate.
		///
		void bind(const frb::Buffer& buffer) noexcept;

		///
		/// Queue a buffer for the source to play.
		///
		/// \param buffer Buffer to queue.
		///
		void queue(const frb::Buffer& buffer) noexcept;

		///
		/// Queue a set of buffers for the source to play through.
		///
		/// \param buffers Array of buffers you want to play in a queue.
		///
		void queue(const std::vector<frb::Buffer*>& buffers) noexcept;

		///
		/// Queue a set of buffers for the source to play through.
		///
		/// \param bufferArray Array of buffers you want to play in a queue.
		/// \param size The size of bufferArray.
		///
		void queue(const ALuint* bufferArray, const size_t size) noexcept;

		///
		/// Play source.
		///
		void play() noexcept;
		
		///
		/// Pause source.
		///
		void pause() noexcept;

		///
		/// Stop source.
		///
		void stop() noexcept;

		///
		/// Rewind source to beginning.
		///
		void rewind() noexcept;

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		const ALuint handle() const noexcept;

		///
		/// Destroy all memory and OpenAL data.
		///
		void destroy() noexcept;

	private:
		///
		/// Handle to source.
		///
		ALuint m_source;
	};
}

#endif