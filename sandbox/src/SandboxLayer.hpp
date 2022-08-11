///
/// SandboxLayer.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_SANDBOX_LAYER_HPP_
#define SANDBOX_SANDBOX_LAYER_HPP_

#include <galaxy/state/Layer.hpp>

using namespace galaxy;

namespace sandbox
{
	class SandboxLayer final : public state::Layer
	{
	public:
		SandboxLayer(std::string_view name, state::Scene* scene) noexcept;
		virtual ~SandboxLayer() noexcept;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update() override;
		void render() override;

		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;

	private:
		SandboxLayer() = delete;
	};
} // namespace sandbox

#endif