///
/// Sandbox.hpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOXTESTS_SANDBOX_HPP_
#define SANDBOXTESTS_SANDBOX_HPP_

#include <protostar/state/State.hpp>

namespace sb
{
	class Sandbox : public pr::State
	{
	public:
		Sandbox();
		virtual ~Sandbox() = default;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;
	};
} // namespace sb

#endif