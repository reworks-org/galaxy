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
	//std::ofstream(m_saveFilePath) saveFile;
	//cereal::JSONOutputArchive ar(saveFile);

	//createFrameworkSnapshot(ar, source);
}

void Serialize::loadGameSnapshot(entt::DefaultRegistry& destination)
{
	//std::ifstream(/*loadfile path*/);
	//cereal::JSONInputArchive ar(loadFile);
	//loadFrameworkSnapshot(ar, destination);
}
