///
/// Serializer.cpp
/// starlight
///
/// Created by reworks on 09/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/tags/CameraTag.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"

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
		source.snapshot().entities(archive).component<AnimationComponent, TransformComponent, PhysicsComponent, RenderComponent>(archive).tag<CameraTag>(archive).destroyed(archive);
	}

	void Serializer::loadFrameworkSnapshot(const cereal::JSONInputArchive& archive, entt::DefaultRegistry& destination)
	{
		entt::ContinuousLoader<entt::DefaultRegistry::entity_type> loader{ destination };
		loader.entities(archive).component<AnimationComponent, TransformComponent, PhysicsComponent, RenderComponent>(archive).tag<CameraTag>(archive).destroyed(archive).orphans().shrink();
	}
}