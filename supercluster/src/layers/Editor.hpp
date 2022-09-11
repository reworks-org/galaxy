///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_EDITOR_HPP_
#define SUPERCLUSTER_LAYERS_EDITOR_HPP_

#include <galaxy/state/Layer.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

using namespace galaxy;

namespace sc
{
	class Editor final : public state::Layer
	{
	public:
		Editor(std::string_view name, state::Scene* scene) noexcept;
		virtual ~Editor() noexcept;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update() override;
		void render() override;

		void new_project();
		void load_project(std::string_view path);
		void save_project();
		void exit();

	private:
		Editor() = delete;

		void viewport();

		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;
	};
} // namespace sc

#endif