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
				const auto error = alGetError();
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to create OpenAL device.", error));
			}

			m_context = alcCreateContext(m_device, nullptr);
			if (!m_context)
			{
				const auto error = alGetError();
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to create OpenAL context.", error));
			}

			if (!alcMakeContextCurrent(m_context))
			{
				const auto error = alGetError();
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Failed to make OpenAL context current.", error));
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
			if (factor < 0.0f)
			{
				GALAXY_LOG(GALAXY_WARNING, "Negative doppler factor, setting to 1.0f.");
				alDopplerFactor(1.0f);
			}
			else
			{
				alDopplerFactor(factor);
			}
		}

		void Context::set_speed_of_sound(const float speed)
		{
			if (speed < 0.0f)
			{
				GALAXY_LOG(GALAXY_WARNING, "Negative speed of sound not applied.");
			}
			else
			{
				alSpeedOfSound(speed);
			}
		}

		void Context::set_listener_gain(const float gain)
		{
			if (gain < 0.0f)
			{
				GALAXY_LOG(GALAXY_WARNING, "Negative audio gain multiplier, setting to 1.0f.");
			}
			else
			{
				alListenerf(AL_GAIN, gain);
			}
		}

		void Context::set_listener_position(const glm::vec3& pos) noexcept
		{
			alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		}

		void Context::set_listener_velocity(const glm::vec3& vel) noexcept
		{
			alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
		}

		void Context::set_listener_orientation(const glm::vec3& at, const glm::vec3& up) noexcept
		{
			const float arr[6] = {at.x, at.y, at.z, up.x, up.y, up.z};
			alListenerfv(AL_ORIENTATION, arr);
		}

		float Context::get_dopper_factor() noexcept
		{
			return alGetFloat(AL_DOPPLER_FACTOR);
		}

		float Context::get_speed_of_sound() noexcept
		{
			return alGetFloat(AL_SPEED_OF_SOUND);
		}

		float Context::get_listener_gain() noexcept
		{
			float gain = 0.0f;
			alGetListenerf(AL_GAIN, &gain);

			return gain;
		}

		glm::vec3 Context::get_listener_position() noexcept
		{
			glm::vec3 pos;
			alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);

			return pos;
		}

		glm::vec3 Context::get_listener_velocity() noexcept
		{
			glm::vec3 vel;
			alGetListener3f(AL_VELOCITY, &vel.x, &vel.y, &vel.z);

			return vel;
		}
	} // namespace audio
} // namespace galaxy