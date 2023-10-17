///
/// Menu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_MENU_HPP_
#define SUPERCLUSTER_LAYERS_MENU_HPP_

#include <galaxy/graphics/Texture.hpp>
#include <galaxy/scene/Scene.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

using namespace galaxy;

namespace sc
{
	class Menu final : public scene::Scene
	{
	  public:
		Menu(const std::string& name);
		virtual ~Menu();

		void load() override;
		void unload() override;
		void update() override;
		void render() override;

	  private:
		void load_project();

	  private:
		graphics::Texture m_bg;
		bool              m_load;
	};
} // namespace sc

#endif
