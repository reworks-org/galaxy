///
/// Serialize.cpp
/// sandbox
///
/// Created by reworks on 20/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "Serialize.hpp"

Serialize::Serialize(const std::string& path)
	:Serializer(path)
{
}

void Serialize::createGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& source)
{
	std::ofstream saveFile; 
	saveFile.open(m_saveFilePath + saveFileName, std::ofstream::out);
	
	{
		cereal::JSONOutputArchive ar(saveFile);
		createFrameworkSnapshot(ar, source);
	}
	
	saveFile.close();
}

void Serialize::loadGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& destination)
{
	std::ifstream saveFile;
	saveFile.open(m_saveFilePath + saveFileName, std::ifstream::in);

	{
		cereal::JSONInputArchive ar(saveFile);
		loadFrameworkSnapshot(ar, destination);
	}

	saveFile.close();
}
