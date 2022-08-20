///
/// Flag.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Flag.hpp"

namespace galaxy
{
	namespace components
	{
		Flag::Flag() noexcept
			: Serializable {}
		{
		}

		Flag::Flag(const nlohmann::json& json) noexcept
			: Serializable {}
		{
			deserialize(json);
		}

		Flag::Flag(Flag&& t) noexcept
			: Serializable {}
		{
			this->m_flags = std::move(t.m_flags);
		}

		Flag& Flag::operator=(Flag&& t) noexcept
		{
			if (this != &t)
			{
				this->m_flags = std::move(t.m_flags);
			}

			return *this;
		}

		Flag::~Flag() noexcept
		{
		}

		nlohmann::json Flag::serialize()
		{
			nlohmann::json json = "{}"_json;

			for (auto i = 0; i < GALAXY_FLAG_BITSET_COUNT; i++)
			{
				json["flags"][std::to_string(i)] = m_flags.test(i);
			}

			return json;
		}

		void Flag::deserialize(const nlohmann::json& json)
		{
			const auto& flags = json.at("flags");
			for (const auto& [index, value] : flags.items())
			{
				m_flags.set(std::stoi(index), value);
			}
		}
	} // namespace components
} // namespace galaxy