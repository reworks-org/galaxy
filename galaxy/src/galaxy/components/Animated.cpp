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
		Animated::Animated() noexcept
		    : Serializable {this}, m_active_anim {nullptr}, m_paused {true}, m_time_spent_on_frame {0.0}
		{
		}

		Animated::Animated(const nlohmann::json& json)
		    : Serializable {this}, m_active_anim {nullptr}, m_paused {true}, m_time_spent_on_frame {0.0}
		{
			deserialize(json);
		}

		Animated::Animated(Animated&& a) noexcept
		    : Serializable {this}
		{
			this->m_active_anim         = a.m_active_anim;
			this->m_animations          = std::move(a.m_animations);
			this->m_paused              = a.m_paused;
			this->m_time_spent_on_frame = a.m_time_spent_on_frame;
		}

		Animated& Animated::operator=(Animated&& a) noexcept
		{
			if (this != &a)
			{
				this->m_active_anim         = a.m_active_anim;
				this->m_animations          = std::move(a.m_animations);
				this->m_paused              = a.m_paused;
				this->m_time_spent_on_frame = a.m_time_spent_on_frame;
			}

			return *this;
		}

		Animated::~Animated() noexcept
		{
			m_animations.clear();
		}

		void Animated::set_animation(std::string_view animation)
		{
			// Reset current anim first.
			if (m_active_anim != nullptr)
			{
				m_active_anim->restart();
			}

			// Then update to new anim.
			m_active_anim = &m_animations[static_cast<std::string>(animation)];
			m_active_anim->restart();
		}

		void Animated::play() noexcept
		{
			if (m_active_anim != nullptr)
			{
				m_paused = false;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to set animation before playing.");
			}
		}

		void Animated::play(std::string_view animation) noexcept
		{
			m_paused = false;
			set_animation(animation);
		}

		void Animated::pause() noexcept
		{
			m_paused = true;
		}

		void Animated::stop() noexcept
		{
			if (m_active_anim != nullptr)
			{
				m_time_spent_on_frame = 0.0;
				m_paused              = true;

				m_active_anim->restart();
			}
		}

		graphics::Animation* const Animated::get_cur_animation() const noexcept
		{
			return m_active_anim;
		}

		auto Animated::get_all() const noexcept -> const robin_hood::unordered_map<std::string, graphics::Animation>&
		{
			return m_animations;
		}

		const bool Animated::is_paused() const noexcept
		{
			return m_paused;
		}

		nlohmann::json Animated::serialize()
		{
			nlohmann::json json        = "{}"_json;
			json["play"]               = m_paused;
			json["starting-animation"] = m_active_anim->get_name();
			json["animations"]         = nlohmann::json::object();

			for (auto& [name, obj] : m_animations)
			{
				json["animations"][name] = obj.serialize();
			}

			return json;
		}

		void Animated::deserialize(const nlohmann::json& json)
		{
			m_active_anim         = nullptr;
			m_paused              = false;
			m_time_spent_on_frame = 0.0;
			m_animations.clear();

			const bool _play                = json.at("play");
			const std::string starting_anim = json.at("starting-animation");

			const auto anim_objs = json.at("animations");
			for (const auto& [name, obj] : anim_objs.items())
			{
				m_animations.emplace(name, graphics::Animation {name, obj});
			}

			set_animation(starting_anim);
			if (_play)
			{
				play();
			}
		}
	} // namespace components
} // namespace galaxy