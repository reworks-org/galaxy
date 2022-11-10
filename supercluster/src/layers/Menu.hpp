///
/// Menu.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_MENU_HPP_
#define SUPERCLUSTER_LAYERS_MENU_HPP_

#include <galaxy/graphics/Texture.hpp>
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

		const std::string& get_type() const noexcept override;
		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;

		void load_project();

	private:
		graphics::Texture m_bg;
		bool m_load;
	};
} // namespace sc

#endif