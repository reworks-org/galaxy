///
/// Animated.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Animations.hpp"

#include "Animated.hpp"

namespace galaxy
{
	namespace components
	{
		Animated::Animated()
			: Serializable {}
			, m_paused {false}
			, m_duration {0.0}
			, m_anim {nullptr}
		{
		}

		Animated::Animated(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Animated::Animated(Animated&& a)
			: Serializable {}
		{
			this->m_paused   = a.m_paused;
			this->m_duration = a.m_duration;
			this->m_anim     = a.m_anim;

			a.m_anim = nullptr;
		}

		Animated& Animated::operator=(Animated&& a)
		{
			if (this != &a)
			{
				this->m_paused   = a.m_paused;
				this->m_duration = a.m_duration;
				this->m_anim     = a.m_anim;

				a.m_anim = nullptr;
			}

			return *this;
		}

		Animated::~Animated()
		{
			m_anim = nullptr;
		}

		void Animated::set_animation(const std::string& animation)
		{
			auto& cache = core::ServiceLocator<resource::Animations>::ref();
			m_anim      = cache.get(animation);
		}

		nlohmann::json Animated::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["paused"]   = m_paused;
			json["duration"] = m_duration;

			if (m_anim)
			{
				json["animation"] = m_anim->m_name;
			}
			else
			{
				json["no_animation"] = true;
			}

			return json;
		}

		void Animated::deserialize(const nlohmann::json& json)
		{
			m_paused   = json.at("paused");
			m_duration = json.at("duration");

			if (!(json.count("no_animation") > 0))
			{
				set_animation(json.at("animation"));
			}
		}
	} // namespace components
} // namespace galaxy
