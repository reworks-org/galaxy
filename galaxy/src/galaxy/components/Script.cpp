///
/// Script.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Script.hpp"

namespace galaxy
{
	namespace components
	{
		Script::Script()
			: Serializable {}
		{
		}

		Script::Script(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Script::Script(Script* ptr)
			: Serializable {}
		{
			load(ptr->file());
		}

		Script::Script(Script&& s)
		{
			this->m_file           = std::move(s.m_file);
			this->m_self           = std::move(s.m_self);
			this->m_update         = std::move(s.m_update);
			this->m_show_functions = s.m_show_functions;
			this->m_show_userdata  = s.m_show_userdata;
			this->m_show_unknown   = s.m_show_unknown;
		}

		Script& Script::operator=(Script&& s)
		{
			if (this != &s)
			{
				this->m_file           = std::move(s.m_file);
				this->m_self           = std::move(s.m_self);
				this->m_update         = std::move(s.m_update);
				this->m_show_functions = s.m_show_functions;
				this->m_show_userdata  = s.m_show_userdata;
				this->m_show_unknown   = s.m_show_unknown;
			}

			return *this;
		}

		Script::~Script()
		{
		}

		void Script::load(std::string_view file)
		{
			if (m_self.valid())
			{
				sol::function destruct = m_self["destruct"];
				if (destruct.valid())
				{
					destruct(m_self);
				}

				m_self.abandon();
			}

			m_file = file;

			if (!core::ServiceLocator<fs::VirtualFileSystem>::ref().contains(m_file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find script '{0}'.", m_file);
				m_file = {};
			}
		}

		const std::string& Script::file() const
		{
			return m_file;
		}

		nlohmann::json Script::serialize()
		{
			nlohmann::json json    = "{}"_json;
			json["file"]           = m_file;
			json["show_functions"] = m_show_functions;
			json["show_userdata"]  = m_show_userdata;
			json["show_unknown"]   = m_show_unknown;

			return json;
		}

		void Script::deserialize(const nlohmann::json& json)
		{
			load(json.at("file"));

			if (json.contains("show_functions"))
			{
				m_show_functions = json.at("show_functions");
			}

			if (json.contains("show_userdata"))
			{
				m_show_userdata = json.at("show_userdata");
			}

			if (json.contains("show_unknown"))
			{
				m_show_unknown = json.at("show_unknown");
			}
		}
	} // namespace components
} // namespace galaxy
