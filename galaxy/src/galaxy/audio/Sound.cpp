///
/// Sound.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace audio
	{
		Sound::Sound() noexcept
		    : Serializable {this}, Buffer {}, SourceManipulator {}
		{
		}

		Sound::Sound(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Sound::~Sound()
		{
			destroy();
		}

		void Sound::play()
		{
			alSourcePlay(m_source.handle());
		}

		void Sound::pause()
		{
			alSourcePause(m_source.handle());
		}

		void Sound::stop()
		{
			alSourceStop(m_source.handle());
			alSourceRewind(m_source.handle());
		}

		const bool Sound::load(std::string_view file)
		{
			destroy();

			const auto res = internal_load(file);
			if (res)
			{
				set_max_distance(1000.0f);
				m_source.queue(this);
			}

			return res;
		}

		void Sound::set_looping(const bool looping)
		{
			alSourcei(m_source.handle(), AL_LOOPING, looping);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to set source looping."));
			}
		}

		const bool Sound::get_looping()
		{
			int looping = 0;

			alGetSourcei(m_source.handle(), AL_LOOPING, &looping);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get source looping."));
			}

			return static_cast<bool>(looping);
		}

		nlohmann::json Sound::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["file"]           = m_filename;
			json["looping"]        = get_looping();
			json["pitch"]          = get_pitch();
			json["gain"]           = get_gain();
			json["rolloff-factor"] = get_rolloff_factor();
			json["max-distance"]   = get_max_distance();

			glm::vec3 cone              = get_cone();
			json["cone"]["outer-gain"]  = cone.x;
			json["cone"]["inner-gain"]  = cone.y;
			json["cone"]["inner-angle"] = cone.z;

			glm::vec3 pos    = get_position();
			json["pos"]["x"] = pos.x;
			json["pos"]["y"] = pos.y;
			json["pos"]["z"] = pos.z;

			glm::vec3 vel    = get_velocity();
			json["vel"]["x"] = vel.x;
			json["vel"]["y"] = vel.y;
			json["vel"]["z"] = vel.z;

			glm::vec3 dir    = get_direction();
			json["dir"]["x"] = dir.x;
			json["dir"]["y"] = dir.y;
			json["dir"]["z"] = dir.z;

			bool is_playing = false;
			if (get_state() == AL_PLAYING)
			{
				is_playing = true;
			}
			json["is-playing"] = is_playing;

			return json;
		}

		void Sound::deserialize(const nlohmann::json& json)
		{
			if (load(json.at("file")))
			{
				set_looping(json.at("looping"));
				set_pitch(json.at("pitch"));
				set_gain(json.at("gain"));
				set_rolloff_factor(json.at("rolloff-factor"));
				set_max_distance(json.at("max-distance"));

				const auto& cone_json = json.at("cone");
				set_cone(cone_json.at("outer-gain"), cone_json.at("inner-gain"), cone_json.at("inner-angle"));

				const auto& pos_json = json.at("pos");
				glm::vec3 pos        = {pos_json.at("x"), pos_json.at("y"), pos_json.at("z")};
				set_position(pos);

				const auto& vel_json = json.at("vel");
				glm::vec3 vel        = {vel_json.at("x"), vel_json.at("y"), vel_json.at("z")};
				set_velocity(vel);

				const auto& dir_json = json.at("dir");
				glm::vec3 dir        = {dir_json.at("x"), dir_json.at("y"), dir_json.at("z")};
				set_direction(dir);

				const bool is_playing = json.at("is-playing");
				if (is_playing)
				{
					play();
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to load sound effect: {0}.", std::string {json.at("file")});
			}
		}

		void Sound::destroy()
		{
			stop();
			alSourceUnqueueBuffers(m_source.handle(), 1, &m_buffer);
		}
	} // namespace audio
} // namespace galaxy