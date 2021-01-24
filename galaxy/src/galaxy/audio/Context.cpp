///
/// Context.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "Context.hpp"

namespace galaxy
{
	namespace audio
	{
		Context::Context()
		    : m_device {nullptr}, m_context {nullptr}
		{
			m_device = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
			if (!m_device)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to create OpenAL device."));
			}

			m_context = alcCreateContext(m_device, nullptr);
			if (!m_context)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to create OpenAL context."));
			}

			if (!alcMakeContextCurrent(m_context))
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to make OpenAL context current."));
			}

			// Make sure error code is default flagged.
			alGetError();
		}

		Context::~Context()
		{
			// Cleanup.
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);

			m_context = nullptr;
			m_device  = nullptr;
		}

		void Context::set_doppler_factor(const float factor)
		{
			if (factor < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Cannot have a negative doppler factor.");
			}
			else
			{
				alDopplerFactor(factor);
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set context doppler factor."));
				}
			}
		}

		void Context::set_speed_of_sound(const float speed)
		{
			if (speed < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Cannot have a negative speed of sound.");
			}
			else
			{
				alSpeedOfSound(speed);
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set context sound speed."));
				}
			}
		}

		void Context::set_listener_gain(const float gain)
		{
			if (gain < 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Cannot have a negative audio gain multiplier.");
			}
			else
			{
				alListenerf(AL_GAIN, gain);
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set listener AL_GAIN."));
				}
			}
		}

		void Context::set_listener_position(const glm::vec3& pos) noexcept
		{
			alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set listener AL_POSITION."));
			}
		}

		void Context::set_listener_velocity(const glm::vec3& vel) noexcept
		{
			alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set listener AL_VELOCITY."));
			}
		}

		void Context::set_listener_orientation(const glm::vec3& at, const glm::vec3& up) noexcept
		{
			const float arr[6] = {at.x, at.y, at.z, up.x, up.y, up.z};
			alListenerfv(AL_ORIENTATION, arr);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set listener AL_ORIENTATION."));
			}
		}
	} // namespace audio
} // namespace galaxy