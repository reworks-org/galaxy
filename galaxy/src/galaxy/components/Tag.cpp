///
/// Tag.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Tag.hpp"

namespace galaxy
{
	namespace components
	{
		Tag::Tag() noexcept
			: Serializable {}
			, m_tag {""}
		{
		}

		Tag::Tag(std::string_view tag) noexcept
			: Serializable {}
			, m_tag {tag}
		{
		}

		Tag::Tag(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Tag::Tag(Tag&& t) noexcept
			: Serializable {}
			, m_tag {"null"}
		{
			this->m_tag = std::move(t.m_tag);
		}

		Tag& Tag::operator=(Tag&& t) noexcept
		{
			if (this != &t)
			{
				this->m_tag = std::move(t.m_tag);
			}

			return *this;
		}

		Tag::~Tag() noexcept
		{
		}

		nlohmann::json Tag::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["tag"]         = m_tag;

			return json;
		}

		void Tag::deserialize(const nlohmann::json& json)
		{
			m_tag = json.at("tag");
		}
	} // namespace components
} // namespace galaxy