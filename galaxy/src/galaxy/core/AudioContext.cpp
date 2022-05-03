///
/// AudioContext.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include "galaxy/error/OpenALError.hpp"

#include "AudioContext.hpp"

namespace galaxy
{
	namespace core
	{
		AudioContext::AudioContext()
			: m_device {nullptr}
			, m_context {nullptr}
		{
			m_device = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
			if (!m_device)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_handle_error("Failed to create OpenAL device.", alGetError()));
			}

			m_context = alcCreateContext(m_device, nullptr);
			if (!m_context)
			{
				GALAXY_LOG(GALAXY_FATAL, error::alc_handle_error(m_device, "Failed to create OpenAL context.", alcGetError(m_device)));
			}

			if (!alcMakeContextCurrent(m_context))
			{
				GALAXY_LOG(GALAXY_FATAL, error::alc_handle_error(m_device, "Failed to make OpenAL context current.", alcGetError(m_device)));
			}
		}

		AudioContext::~AudioContext()
		{
			// Cleanup.
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);

			m_context = nullptr;
			m_device  = nullptr;
		}

		void AudioContext::suspend() noexcept
		{
			alcSuspendContext(m_context);
		}

		void AudioContext::resume() noexcept
		{
			alcProcessContext(m_context);
		}

		void AudioContext::set_doppler_factor(const float factor)
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

		void AudioContext::set_speed_of_sound(const float speed)
		{
			if (speed < 0.0f)
			{
				GALAXY_LOG(GALAXY_WARNING, "Negative speed of sound, setting to 1.0f.");
				alSpeedOfSound(1.0f);
			}
			else
			{
				alSpeedOfSound(speed);
			}
		}

		void AudioContext::set_distance_model(const DistanceModel model) noexcept
		{
			alDistanceModel(static_cast<ALenum>(model));
		}

		void AudioContext::set_listener_gain(const float gain)
		{
			if (gain < 0.0f)
			{
				GALAXY_LOG(GALAXY_WARNING, "Negative audio gain multiplier, setting to 1.0f.");
				alListenerf(AL_GAIN, 1.0f);
			}
			else
			{
				alListenerf(AL_GAIN, gain);
			}
		}

		void AudioContext::set_listener_position(const glm::vec3& pos) noexcept
		{
			alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		}

		void AudioContext::set_listener_velocity(const glm::vec3& vel) noexcept
		{
			alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
		}

		void AudioContext::set_listener_orientation(const glm::vec3& at, const glm::vec3& up) noexcept
		{
			const float arr[6] = {at.x, at.y, at.z, up.x, up.y, up.z};
			alListenerfv(AL_ORIENTATION, arr);
		}

		bool AudioContext::has_extension(const ALchar* name) noexcept
		{
			return alIsExtensionPresent(name);
		}

		bool AudioContext::has_context_extension(const ALchar* name) noexcept
		{
			return alcIsExtensionPresent(m_device, name);
		}

		float AudioContext::get_dopper_factor() noexcept
		{
			return alGetFloat(AL_DOPPLER_FACTOR);
		}

		float AudioContext::get_speed_of_sound() noexcept
		{
			return alGetFloat(AL_SPEED_OF_SOUND);
		}

		float AudioContext::get_distance_model() noexcept
		{
			return alGetFloat(AL_DISTANCE_MODEL);
		}

		float AudioContext::get_listener_gain() noexcept
		{
			float gain = 0.0f;
			alGetListenerf(AL_GAIN, &gain);

			return gain;
		}

		glm::vec3 AudioContext::get_listener_position() noexcept
		{
			glm::vec3 pos;
			alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);

			return pos;
		}

		glm::vec3 AudioContext::get_listener_velocity() noexcept
		{
			glm::vec3 vel;
			alGetListener3f(AL_VELOCITY, &vel.x, &vel.y, &vel.z);

			return vel;
		}
	} // namespace core
} // namespace galaxy