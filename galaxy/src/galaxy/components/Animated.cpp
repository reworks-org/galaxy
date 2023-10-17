///
/// Animated.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "Animated.hpp"

namespace galaxy
{
	namespace components
	{
		Animated::Animated()
			: Serializable {}
			, m_active {nullptr}
			, m_paused {true}
			, m_time_spent_on_frame {0.0}
		{
		}

		Animated::Animated(const nlohmann::json& json)
			: Serializable {}
			, m_active {nullptr}
			, m_paused {true}
			, m_time_spent_on_frame {0.0}
		{
			deserialize(json);
		}

		Animated::Animated(Animated* ptr)
			: Serializable {}
			, m_active {nullptr}
			, m_paused {true}
			, m_time_spent_on_frame {0.0}
		{
			const auto& copy = *ptr;

			this->m_animations = copy.m_animations;
		}

		Animated::Animated(Animated&& a)
			: Serializable {}
			, m_active {nullptr}
			, m_paused {true}
			, m_time_spent_on_frame {0.0}
		{
			this->m_active              = a.m_active;
			this->m_paused              = a.m_paused;
			this->m_time_spent_on_frame = a.m_time_spent_on_frame;
			this->m_animations          = std::move(a.m_animations);
		}

		Animated& Animated::operator=(Animated&& a)
		{
			if (this != &a)
			{
				this->m_active              = a.m_active;
				this->m_paused              = a.m_paused;
				this->m_time_spent_on_frame = a.m_time_spent_on_frame;
				this->m_animations          = std::move(a.m_animations);
			}

			return *this;
		}

		Animated::~Animated()
		{
			m_animations.clear();
		}

		void Animated::add(const std::string& name, const bool looping, const double speed, std::span<graphics::Frame> frames)
		{
			if (!m_animations.contains(name))
			{
				m_animations[name] = graphics::Animation(name, looping, speed, frames);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to add duplicate animation '{0}'.", name);
			}
		}

		void Animated::set(const std::string& animation)
		{
			// Reset current anim first.
			if (m_active != nullptr)
			{
				m_active->restart();
			}

			// Then update to new anim.
			m_active = &m_animations[animation];
			m_active->restart();
		}

		void Animated::play()
		{
			if (m_active != nullptr)
			{
				m_paused = false;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to set animation before playing.");
			}
		}

		void Animated::play(const std::string& animation)
		{
			m_paused = false;
			set(animation);
		}

		void Animated::pause()
		{
			m_paused = true;
		}

		void Animated::stop()
		{
			if (m_active != nullptr)
			{
				m_time_spent_on_frame = 0.0;
				m_paused              = true;

				m_active->restart();
			}
		}

		bool Animated::is_paused() const
		{
			return m_paused;
		}

		graphics::Animation* Animated::active()
		{
			return m_active;
		}

		nlohmann::json Animated::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["playing"]     = m_paused;
			json["starting"]    = m_active->m_name;
			json["animations"]  = nlohmann::json::object();

			for (auto& [name, obj] : m_animations)
			{
				json["animations"][name] = obj.serialize();
			}

			return json;
		}

		void Animated::deserialize(const nlohmann::json& json)
		{
			m_active              = nullptr;
			m_paused              = false;
			m_time_spent_on_frame = 0.0;
			m_animations.clear();

			const auto& anims = json.at("animations");
			for (const auto& [name, obj] : anims.items())
			{
				m_animations.emplace(name, obj);
			}

			set(json.at("starting"));

			const bool playing = json.at("playing");
			if (playing)
			{
				play();
			}
		}
	} // namespace components
} // namespace galaxy
