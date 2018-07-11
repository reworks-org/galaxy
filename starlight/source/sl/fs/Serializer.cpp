///
/// Serializer.cpp
/// starlight
///
/// Created by reworks on 09/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Serializer.hpp"

#include "sl/core/StateMachine.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/resources/MusicPlayer.hpp"

namespace sl
{
	Serializer::Serializer(const std::string& saveFilePath)
	:m_saveFilePath(saveFilePath)
	{
	}

	void Serializer::createFrameworkSnapshot(cereal::JSONOutputArchive& oarchive, entt::DefaultRegistry& source)
	{
		// Serialise current music position.
		for (auto& pair : Locator::musicPlayer->m_resourceMap)
		{
			oarchive(pair.second);
		}

		// Serialize the state machine
		oarchive(*(Locator::stateMachine));
	}

	void Serializer::loadFrameworkSnapshot(cereal::JSONInputArchive& iarchive, entt::DefaultRegistry& destination)
	{
		// Deserialise current music position.
		for (auto& pair : Locator::musicPlayer->m_resourceMap)
		{
			iarchive(pair.second);
		}

		// Deserialize the state machine
		iarchive(*(Locator::stateMachine));
	}
}