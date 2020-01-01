///
/// Source.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOURCE_HPP_
#define FRB_SOURCE_HPP_

#include "frb/audio/Buffer.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Origin of the audio sound.
	///
	class Source
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Can throw exceptions.
		/// Generates source buffer(s).
		///
		Source();

		///
		/// \brief Default destructor.
		///
		/// Destroys source buffer(s).
		///
		~Source() noexcept;

		///
		/// \brief Audio pitch.
		///
		/// Can throw exceptions.
		/// Negative values automatically converted to positive.
		///
		/// \param pitch Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void setPitch(const float pitch);

		///
		/// \brief Audio gain.
		///
		/// Can throw exceptions.
		/// Negative values automatically converted to positive.
		///
		/// \param gain Multiplier value. I.e. 0.5f is a *0.5 multiplier.
		///
		void setGain(const float gain);

		///
		/// \brief Specifies the current location of the object in the world coordinatesystem.
		///
		/// Can throw exceptions.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setPosition(const float x, const float y, const float z);
		
		///
		/// \brief Specifies the current velocity (speed and direction) of the object, in theworld coordinate system
		///
		/// Can throw exceptions.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setVelocity(const float x, const float y, const float z);

		///
		/// \brief Source is directional. The sound emission is presumed to be symmetric around the direction vector.
		///
		/// Can throw exceptions.
		///
		/// \param x position in world.
		/// \param y position in world.
		/// \param z position in world.
		///
		void setDirection(const float x, const float y, const float z);

		///
		/// \brief Should the source repeat upon reaching the end.
		///
		/// Can throw exceptions.
		///
		/// \param True to repeat.
		///
		void setLooping(bool looping);

		///
		/// Bind an audio buffer to an audio source.
		///
		/// \param buffer Buffer audio to play / manipulate.
		///
		void bind(const frb::Buffer& buffer);

		///
		/// Get the OpenAL internal int id / handle.
		///
		/// \return Const ALuint handle integer.
		///
		const ALuint handle() const noexcept;

	private:
		///
		/// Handle to source.
		///
		ALuint m_source;
	};
}

#endif