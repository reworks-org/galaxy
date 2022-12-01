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
		Script::Script() noexcept
		{
		}

		Script::Script(const nlohmann::json& json)
		{
			deserialize(json);
		}

		Script::Script(Script&& s) noexcept
		{
			this->m_file   = std::move(s.m_file);
			this->m_self   = std::move(s.m_self);
			this->m_update = std::move(s.m_update);
		}

		Script& Script::operator=(Script&& s) noexcept
		{
			if (this != &s)
			{
				this->m_file   = std::move(s.m_file);
				this->m_self   = std::move(s.m_self);
				this->m_update = std::move(s.m_update);
			}

			return *this;
		}

		Script::~Script() noexcept
		{
		}

		void Script::load(std::string_view file) noexcept
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				m_file      = info.string;
				auto& state = core::ServiceLocator<sol::state>::ref();

				auto result = state.load_file(m_file);
				if (result.valid())
				{
					m_self = result.call();
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}' because '{1}'.", file, magic_enum::enum_name(result.status()));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find script '{0}' because '{1}'.", file, magic_enum::enum_name(info.code));
			}
		}

		const std::string& Script::file() const noexcept
		{
			return m_file;
		}

		nlohmann::json Script::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["file"]        = m_file;

			return json;
		}

		void Script::deserialize(const nlohmann::json& json)
		{
			load(json.at("file").get<std::string>());
		}
	} // namespace components
} // namespace galaxy