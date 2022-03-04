///
/// SourceManipulator.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "SourceManipulator.hpp"

namespace galaxy
{
	namespace audio
	{
		void SourceManipulator::set_pitch(const float pitch) noexcept
		{
			alSourcef(m_source.handle(), AL_PITCH, std::clamp(pitch, 0.0f, 10.0f));
		}

		void SourceManipulator::set_gain(const float gain) noexcept
		{
			alSourcef(m_source.handle(), AL_GAIN, std::clamp(gain, 0.0f, 10.0f));
		}

		void SourceManipulator::set_rolloff_factor(const float factor) noexcept
		{
			alSourcef(m_source.handle(), AL_ROLLOFF_FACTOR, std::clamp(factor, 0.0f, 10.0f));
		}

		void SourceManipulator::set_max_distance(const float distance) noexcept
		{
			alSourcef(m_source.handle(), AL_MAX_DISTANCE, distance);
		}

		void SourceManipulator::set_cone(const float outer_gain, const float inner_gain, const float outer_angle) noexcept
		{
			alSourcef(m_source.handle(), AL_CONE_OUTER_GAIN, outer_gain);
			alSourcef(m_source.handle(), AL_CONE_INNER_ANGLE, inner_gain);
			alSourcef(m_source.handle(), AL_CONE_OUTER_ANGLE, outer_angle);
		}

		void SourceManipulator::set_position(const glm::vec3& pos) noexcept
		{
			alSource3f(m_source.handle(), AL_POSITION, pos.x, pos.y, pos.z);
		}

		void SourceManipulator::set_velocity(const glm::vec3& vel) noexcept
		{
			alSource3f(m_source.handle(), AL_VELOCITY, vel.x, vel.y, vel.z);
		}

		void SourceManipulator::set_direction(const glm::vec3& dir) noexcept
		{
			alSource3f(m_source.handle(), AL_DIRECTION, dir.x, dir.y, dir.z);
		}

		ALint SourceManipulator::get_state() noexcept
		{
			int val = 0;
			alGetSourcei(m_source.handle(), AL_SOURCE_STATE, &val);

			return val;
		}

		float SourceManipulator::get_pitch() noexcept
		{
			float val;
			alGetSourcef(m_source.handle(), AL_PITCH, &val);

			return val;
		}

		float SourceManipulator::get_gain() noexcept
		{
			float val;
			alGetSourcef(m_source.handle(), AL_GAIN, &val);

			return val;
		}

		float SourceManipulator::get_rolloff_factor() noexcept
		{
			float val;
			alGetSourcef(m_source.handle(), AL_ROLLOFF_FACTOR, &val);

			return val;
		}

		float SourceManipulator::get_max_distance() noexcept
		{
			float val;
			alGetSourcef(m_source.handle(), AL_MAX_DISTANCE, &val);

			return val;
		}

		glm::vec3 SourceManipulator::get_cone() noexcept
		{
			glm::vec3 val;
			alGetSourcef(m_source.handle(), AL_CONE_OUTER_GAIN, &val.x);
			alGetSourcef(m_source.handle(), AL_CONE_INNER_ANGLE, &val.y);
			alGetSourcef(m_source.handle(), AL_CONE_OUTER_ANGLE, &val.z);

			return val;
		}

		glm::vec3 SourceManipulator::get_position() noexcept
		{
			glm::vec3 val;
			alGetSource3f(m_source.handle(), AL_POSITION, &val.x, &val.y, &val.z);

			return val;
		}

		glm::vec3 SourceManipulator::get_velocity() noexcept
		{
			glm::vec3 val;
			alGetSource3f(m_source.handle(), AL_VELOCITY, &val.x, &val.y, &val.z);

			return val;
		}

		glm::vec3 SourceManipulator::get_direction() noexcept
		{
			glm::vec3 val;
			alGetSource3f(m_source.handle(), AL_DIRECTION, &val.x, &val.y, &val.z);

			return val;
		}
	} // namespace audio
} // namespace galaxy