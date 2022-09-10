///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_Editor_HPP_
#define SUPERCLUSTER_LAYERS_Editor_HPP_

#include <nlohmann/json.hpp>

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

	private:
		Editor() = delete;

		[[nodiscard]] inline nlohmann::json serialize() override
		{
			return {};
		}

		inline void deserialize(const nlohmann::json& json) override
		{
		}
	};
} // namespace sc

#endif