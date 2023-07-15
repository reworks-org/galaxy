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
		Tag::Tag()
			: Serializable {}
			, m_tag {""}
		{
		}

		Tag::Tag(std::string_view tag)
			: Serializable {}
			, m_tag {tag}
		{
		}

		Tag::Tag(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Tag::Tag(Tag* ptr)
			: Serializable {}
		{
			const auto& copy = *ptr;

			this->m_tag = copy.m_tag;
		}

		Tag::Tag(Tag&& t)
			: Serializable {}
			, m_tag {"null"}
		{
			this->m_tag = std::move(t.m_tag);
		}

		Tag& Tag::operator=(Tag&& t)
		{
			if (this != &t)
			{
				this->m_tag = std::move(t.m_tag);
			}

			return *this;
		}

		Tag::~Tag()
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