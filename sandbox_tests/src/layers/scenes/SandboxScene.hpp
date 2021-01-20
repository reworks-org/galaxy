///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SCENES_SANDBOXSCENE_HPP_
#define SANDBOXTESTS_LAYERS_SCENES_SANDBOXSCENE_HPP_

#include <galaxy/core/Scene.hpp>

namespace sb
{
	class SandboxScene final : public galaxy::core::Scene
	{
	public:
		SandboxScene();
		virtual ~SandboxScene();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;
	};
} // namespace sb

#endif