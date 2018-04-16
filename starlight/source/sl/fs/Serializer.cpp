///
/// Serializer.cpp
/// starlight
///
/// Created by reworks on 09/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/cereal/archives/json.hpp"

#include "Serializer.hpp"

namespace sl
{
	Serializer::Serializer(const std::string& saveFilePath) noexcept
	:m_saveFilePath(saveFilePath)
	{
	}

	void Serializer::createFrameworkSnapshot(cereal::JSONOutputArchive& archive, const entt::DefaultRegistry& source)
	{
		source.snapshot().entities(archive).destroyed(archive).component<>(archive).tag<>(archive);
	}

	void Serializer::loadFrameworkSnapshot(const cereal::JSONInputArchive& archive, entt::DefaultRegistry& destination)
	{
	}
}