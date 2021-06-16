///
/// Tag.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Tag.hpp"

namespace galaxy
{
	namespace components
	{
		Tag::Tag() noexcept
		    : Serializable {this}, m_tag {"null"}
		{
		}

		Tag::Tag(std::string_view tag) noexcept
		    : Serializable {this}, m_tag {tag}
		{
		}

		Tag::Tag(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Tag::Tag(Tag&& t) noexcept
		    : Serializable {this}, m_tag {"null"}
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