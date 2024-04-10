///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/ui/NuklearUI.hpp"

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
			if (m_self.valid())
			{
				m_self.abandon();
			}
		}

		void GUI::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto script = fs.read(file);
			if (!script.empty())
			{
				if (m_self.valid())
				{
					m_self.abandon();
				}

				m_file = file;

				auto& state = core::ServiceLocator<sol::state>::ref();
				auto& nui   = core::ServiceLocator<ui::NuklearUI>::ref();
				auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();

				if (!script.empty())
				{
					auto result = state.load(script);

					if (result.valid())
					{
						m_self = result.call();

						if (m_self.valid())
						{
							m_self["ctx"] = nui.ctx();
							m_update      = m_self["do_ui"];

							if (!m_update.valid())
							{
								GALAXY_LOG(GALAXY_ERROR, "Update function not present in ui script '{0}'.", m_file);
							}
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to validate ui script '{0}'. Make sure its in the correct format.", m_file);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load ui script '{0}' because '{1}'.", m_file, magic_enum::enum_name(result.status()));
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read script '{0}'.", m_file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find ui script '{0}'.", m_file);
			}
		}

		const std::string& GUI::file() const
		{
			return m_file;
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
