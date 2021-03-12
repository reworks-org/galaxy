///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_LAYERS_SANDBOX_HPP_
#define SANDBOXTESTS_LAYERS_SANDBOX_HPP_

#include <galaxy/core/Instance.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Camera.hpp>

namespace sb
{
	class Sandbox final : public galaxy::core::Instance
	{
	public:
		Sandbox();
		virtual ~Sandbox();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		galaxy::core::Window* m_window;
	};
} // namespace sb

#endif