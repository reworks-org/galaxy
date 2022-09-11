///
/// Menu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_MENU_HPP_
#define SUPERCLUSTER_LAYERS_MENU_HPP_

#include <galaxy/state/Layer.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

using namespace galaxy;

namespace sc
{
	class Menu final : public state::Layer
	{
	public:
		Menu(std::string_view name, state::Scene* scene) noexcept;
		virtual ~Menu() noexcept;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update() override;
		void render() override;

	private:
		Menu() = delete;

		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;

	private:
		ImFont* m_bigger_default_font;
	};
} // namespace sc

#endif