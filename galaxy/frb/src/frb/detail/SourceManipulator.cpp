///
/// SourceManipulator.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/al.h>
#include <AL/alc.h>
#include <pulsar/Log.hpp>

#include "frb/detail/Error.hpp"

#include "SourceManipulator.hpp"

///
/// Core namespace.
///
namespace frb
{
	void SourceManipulator::play() noexcept
	{
		alSourcePlay(m_source);
	}

	void SourceManipulator::pause() noexcept
	{
		alSourcePause(m_source);
	}

	void SourceManipulator::stop() noexcept
	{
		alSourceStop(m_source);
	}

	void SourceManipulator::rewind() noexcept
	{
		alSourceRewind(m_source);
	}

	void SourceManipulator::set_pitch(pr::positive_float auto pitch) noexcept
	{
		alSourcef(m_source, AL_PITCH, std::abs(pitch));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source pitch."));
		}
	}

	void SourceManipulator::set_gain(pr::positive_float auto gain) noexcept
	{
		alSourcef(m_source, AL_GAIN, std::abs(gain));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source gain."));
		}
	}

	void SourceManipulator::set_rolloff_factor(pr::positive_float auto factor) noexcept
	{
		alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source rolloff."));
		}
	}

	void SourceManipulator::set_max_distance(const float distance) noexcept
	{
		alSourcef(m_source, AL_MAX_DISTANCE, distance);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source max distance."));
		}
	}

	void SourceManipulator::set_cone(const float outer_gain, const float inner_angle, const float outer_angle) noexcept
	{
		alSourcef(m_source, AL_CONE_OUTER_GAIN, outer_gain);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_OUTER_GAIN."));
		}

		alSourcef(m_source, AL_CONE_INNER_ANGLE, inner_angle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_INNER_ANGLE."));
		}

		alSourcef(m_source, AL_CONE_OUTER_ANGLE, outer_angle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_OUTER_ANGLE."));
		}
	}

	void SourceManipulator::set_position(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source position."));
		}
	}

	void SourceManipulator::set_velocity(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source velocity."));
		}
	}

	void SourceManipulator::set_direction(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source direction."));
		}
	}

	void SourceManipulator::set_looping(bool looping) noexcept
	{
		alSourcei(m_source, AL_LOOPING, looping);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source looping."));
		}
	}

	SourceManipulator::SourceManipulator() noexcept
	    : m_source {0}
	{
	}

	void SourceManipulator::set_source_to_manipulate(const pr::positive_uint auto) noexcept
	{
		m_source = source;
	}
} // namespace frb