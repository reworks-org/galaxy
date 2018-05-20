///
/// Serialize.cpp
/// sandbox
///
/// Created by reworks on 20/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Serialize.hpp"

Serialize::Serialize(const std::string& path)
	:Serializer(path)
{
}

void Serialize::createGameSnapshot(entt::DefaultRegistry& source)
{
	//createFrameworkSnapshot(, source);
}

void Serialize::loadGameSnapshot(entt::DefaultRegistry& destination)
{
	//loadFrameworkSnapshot(, destination);
}
