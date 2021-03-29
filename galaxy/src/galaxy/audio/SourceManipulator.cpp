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

		void SourceManipulator::set_cone(const float outer_gain, const float inner_gain, const float outer_angle)
		{
			alSourcef(m_source.handle(), AL_CONE_OUTER_GAIN, outer_gain);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set AL_CONE_OUTER_GAIN."));
			}

			alSourcef(m_source.handle(), AL_CONE_INNER_ANGLE, inner_gain);
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
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source state."));
			}

			return val;
		}

		const float SourceManipulator::get_pitch()
		{
			float val = 0.0f;

			alGetSourcef(m_source.handle(), AL_PITCH, &val);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source pitch."));
			}

			return val;
		}

		const float SourceManipulator::get_gain()
		{
			float val = 0.0f;

			alGetSourcef(m_source.handle(), AL_GAIN, &val);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source gain."));
			}

			return val;
		}

		const float SourceManipulator::get_rolloff_factor()
		{
			float val = 0.0f;

			alGetSourcef(m_source.handle(), AL_ROLLOFF_FACTOR, &val);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source rolloff factor."));
			}

			return val;
		}

		const float SourceManipulator::get_max_distance()
		{
			float val = 0.0f;

			alGetSourcef(m_source.handle(), AL_MAX_DISTANCE, &val);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source max distance."));
			}

			return val;
		}

		glm::vec3 SourceManipulator::get_cone()
		{
			glm::vec3 val;

			alGetSourcef(m_source.handle(), AL_CONE_OUTER_GAIN, &val.x);
			alGetSourcef(m_source.handle(), AL_CONE_INNER_ANGLE, &val.y);
			alGetSourcef(m_source.handle(), AL_CONE_OUTER_ANGLE, &val.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source cone."));
			}

			return val;
		}

		glm::vec3 SourceManipulator::get_position()
		{
			glm::vec3 val;

			alGetSource3f(m_source.handle(), AL_POSITION, &val.x, &val.y, &val.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source position."));
			}

			return val;
		}

		glm::vec3 SourceManipulator::get_velocity()
		{
			glm::vec3 val;

			alGetSource3f(m_source.handle(), AL_VELOCITY, &val.x, &val.y, &val.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source velocity."));
			}

			return val;
		}

		glm::vec3 SourceManipulator::get_direction()
		{
			glm::vec3 val;

			alGetSource3f(m_source.handle(), AL_DIRECTION, &val.x, &val.y, &val.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source direction."));
			}

			return val;
		}
	} // namespace audio
} // namespace galaxy