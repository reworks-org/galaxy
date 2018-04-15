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

/*
	https://github.com/skypjack/entt/blob/master/test/snapshot/snapshot.cpp
*/

namespace sl
{
	Serializer::Serializer(const std::string& saveFilePath) noexcept
	:m_saveFilePath(saveFilePath)
	{
	}

	void Serializer::createFrameworkSnapshot()
	{
	}

	void Serializer::loadFrameworkSnapshot()
	{
	}
}