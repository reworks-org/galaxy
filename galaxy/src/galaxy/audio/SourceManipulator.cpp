///
/// SourceManipulator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "SourceManipulator.hpp"

namespace galaxy
{
	namespace audio
	{
		void SourceManipulator::set_pitch(const float pitch)
		{
			if (pitch < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Pitch cannot be a negative.");
			}
			else
			{
				alSourcef(m_source.handle(), AL_PITCH, std::abs(pitch));
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source pitch."));
				}
			}
		}

		void SourceManipulator::set_gain(const float gain)
		{
			if (gain < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Pitch cannot be a negative.");
			}
			else
			{
				alSourcef(m_source.handle(), AL_GAIN, std::abs(gain));
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source gain."));
				}
			}
		}

		void SourceManipulator::set_rolloff_factor(const float factor)
		{
			if (factor < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Pitch cannot be a negative.");
			}
			else
			{
				alSourcef(m_source.handle(), AL_ROLLOFF_FACTOR, factor);
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source rolloff."));
				}
			}
		}

		void SourceManipulator::set_max_distance(const float distance)
		{
			alSourcef(m_source.handle(), AL_MAX_DISTANCE, distance);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source max distance."));
			}
		}

		void SourceManipulator::set_cone(const float outer_gain, const float inner_angle, const float outer_angle)
		{
			alSourcef(m_source.handle(), AL_CONE_OUTER_GAIN, outer_gain);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set AL_CONE_OUTER_GAIN."));
			}

			alSourcef(m_source.handle(), AL_CONE_INNER_ANGLE, inner_angle);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set AL_CONE_INNER_ANGLE."));
			}

			alSourcef(m_source.handle(), AL_CONE_OUTER_ANGLE, outer_angle);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set AL_CONE_OUTER_ANGLE."));
			}
		}

		void SourceManipulator::set_position(const glm::vec3& pos)
		{
			alSource3f(m_source.handle(), AL_POSITION, pos.x, pos.y, pos.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source position."));
			}
		}

		void SourceManipulator::set_velocity(const glm::vec3& vel)
		{
			alSource3f(m_source.handle(), AL_VELOCITY, vel.x, vel.y, vel.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source velocity."));
			}
		}

		void SourceManipulator::set_direction(const glm::vec3& dir)
		{
			alSource3f(m_source.handle(), AL_DIRECTION, dir.x, dir.y, dir.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source direction."));
			}
		}

		const ALint SourceManipulator::get_state()
		{
			int val = 0;
			alGetSourcei(m_source.handle(), AL_SOURCE_STATE, &val);

			return val;
		}
	} // namespace audio
} // namespace galaxy