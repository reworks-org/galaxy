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
	SourceManipulator::~SourceManipulator()
	{
		m_source = 0;
	}

	void SourceManipulator::play()
	{
		alSourcePlay(m_source);
	}

	void SourceManipulator::pause()
	{
		alSourcePause(m_source);
	}

	void SourceManipulator::stop()
	{
		alSourceStop(m_source);
	}

	void SourceManipulator::rewind()
	{
		alSourceRewind(m_source);
	}

	void SourceManipulator::set_pitch(const float pitch)
	{
		if (pitch < 0)
		{
			throw std::runtime_error("Pitch cannot be a negative.");
		}
		else
		{
			alSourcef(m_source, AL_PITCH, std::abs(pitch));
			if (alGetError() != AL_NO_ERROR)
			{
				PL_LOG(PL_ERROR, frb::parse_error("Unable to set source pitch."));
			}
		}
	}

	void SourceManipulator::set_gain(const float gain)
	{
		if (gain < 0)
		{
			throw std::runtime_error("Pitch cannot be a negative.");
		}
		else
		{
			alSourcef(m_source, AL_GAIN, std::abs(gain));
			if (alGetError() != AL_NO_ERROR)
			{
				PL_LOG(PL_ERROR, frb::parse_error("Unable to set source gain."));
			}
		}
	}

	void SourceManipulator::set_rolloff_factor(const float factor)
	{
		if (factor < 0)
		{
			throw std::runtime_error("Pitch cannot be a negative.");
		}
		else
		{
			alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
			if (alGetError() != AL_NO_ERROR)
			{
				PL_LOG(PL_ERROR, frb::parse_error("Unable to set source rolloff."));
			}
		}
	}

	void SourceManipulator::set_max_distance(const float distance)
	{
		alSourcef(m_source, AL_MAX_DISTANCE, distance);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source max distance."));
		}
	}

	void SourceManipulator::set_cone(const float outer_gain, const float inner_angle, const float outer_angle)
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

	void SourceManipulator::set_position(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source position."));
		}
	}

	void SourceManipulator::set_velocity(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source velocity."));
		}
	}

	void SourceManipulator::set_direction(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source direction."));
		}
	}

	void SourceManipulator::set_looping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source looping."));
		}
	}

	SourceManipulator::SourceManipulator()
	    : m_source {0}
	{
	}

	SourceManipulator::SourceManipulator(SourceManipulator&& sm)
	{
		this->m_source = sm.m_source;
		sm.m_source    = 0;
	}

	SourceManipulator& SourceManipulator::operator=(SourceManipulator&& sm)
	{
		if (this != &sm)
		{
			this->m_source = sm.m_source;
			sm.m_source    = 0;
		}

		return *this;
	}

	void SourceManipulator::set_source_to_manipulate(const ALuint source)
	{
		m_source = source;
	}

	ALint SourceManipulator::get_state()
	{
		int val = 0;
		alGetSourcei(m_source, AL_SOURCE_STATE, &val);

		return val;
	}
} // namespace frb