///
/// UIScript.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "UIScript.hpp"

namespace galaxy
{
	namespace components
	{
		UIScript::UIScript()
		{
		}

		UIScript::UIScript(const nlohmann::json& json)
		{
			deserialize(json);
		}

		UIScript::UIScript(UIScript&& s)
		{
			this->m_self   = std::move(s.m_self);
			this->m_update = std::move(s.m_update);
			this->m_file   = std::move(s.m_file);
		}

		UIScript& UIScript::operator=(UIScript&& s)
		{
			if (this != &s)
			{
				this->m_self   = std::move(s.m_self);
				this->m_update = std::move(s.m_update);
				this->m_file   = std::move(s.m_file);
			}

			return *this;
		}

		UIScript::~UIScript()
		{
		}

		const std::string& UIScript::file() const
		{
			return m_file;
		}

		void UIScript::load(const std::string& file)
		{
			if (m_self.valid())
			{
				m_self.abandon();
			}

			m_file = file;

			if (!core::ServiceLocator<fs::VirtualFileSystem>::ref().contains(m_file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find ui script '{0}'.", m_file);
				m_file = {};
			}
		}

		nlohmann::json UIScript::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["file"]        = m_file;

			return json;
		}

		void UIScript::deserialize(const nlohmann::json& json)
		{
			load(json.at("file"));
		}
	} // namespace components
} // namespace galaxy
