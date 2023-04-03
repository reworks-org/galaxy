///
/// Prefabs.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/ZLib.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Prefabs.hpp"

namespace galaxy
{
	namespace resource
	{
		Prefabs::Prefabs()
			: m_folder {""}
		{
		}

		Prefabs::~Prefabs()
		{
		}

		void Prefabs::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto data = fs.open(file);
					if (!data.empty())
					{
						const auto name         = std::filesystem::path(file).stem().string();
						const auto base64       = algorithm::decode_zlib(data);
						const auto decompressed = algorithm::decode_base64(base64);

						m_cache[name]         = std::make_shared<core::Prefab>();
						m_cache[name]->m_data = nlohmann::json::parse(decompressed);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no Prefabs to load in '{0}'.", m_folder);
			}
		}
	} // namespace resource
} // namespace galaxy