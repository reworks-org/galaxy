///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "GUI.hpp"

namespace galaxy
{
	namespace components
	{
		GUI::GUI()
		{
		}

		GUI::GUI(const nlohmann::json& json)
		{
			deserialize(json);
		}

		GUI::GUI(GUI&& s)
		{
			this->m_self   = std::move(s.m_self);
			this->m_update = std::move(s.m_update);
			this->m_file   = std::move(s.m_file);
		}

		GUI& GUI::operator=(GUI&& s)
		{
			if (this != &s)
			{
				this->m_self   = std::move(s.m_self);
				this->m_update = std::move(s.m_update);
				this->m_file   = std::move(s.m_file);
			}

			return *this;
		}

		GUI::~GUI()
		{
		}

		const std::string& GUI::file() const
		{
			return m_file;
		}

		void GUI::load(const std::string& file)
		{
			if (m_self.valid())
			{
				m_self.abandon();
			}

			m_file = file;

			if (!core::ServiceLocator<fs::VirtualFileSystem>::ref().exists(m_file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find ui script '{0}'.", m_file);
				m_file = {};
			}
		}

		nlohmann::json GUI::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["file"]        = m_file;

			return json;
		}

		void GUI::deserialize(const nlohmann::json& json)
		{
			load(json.at("file"));
		}
	} // namespace components
} // namespace galaxy
