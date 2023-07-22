///
/// RMLSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Flag.hpp"
#include "galaxy/components/RML.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/scene/Scene.hpp"

#include "RMLSystem.hpp"

namespace galaxy
{
	namespace systems
	{
		RMLSystem::RMLSystem()
		{
		}

		RMLSystem::~RMLSystem()
		{
		}

		void RMLSystem::update(scene::Scene* scene)
		{
			const auto view = scene->m_world.m_registry.view<components::RML, components::Flag>();
			for (auto&& [entity, rml, flag] : view.each())
			{
				if (rml.m_doc)
				{
					if (flag.is_flag_set<flags::Enabled>())
					{
						if (!rml.m_doc->IsVisible())
						{
							rml.m_doc->Show();
						}
					}
					else
					{
						if (rml.m_doc->IsVisible())
						{
							rml.m_doc->Hide();
						}
					}
				}
			}
		}
	} // namespace systems
} // namespace galaxy