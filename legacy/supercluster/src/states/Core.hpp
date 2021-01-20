///
/// Core.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_STATES_HPP_
#define SUPERCLUSTER_STATES_HPP_

#include <galaxy/core/State.hpp>

namespace sc
{
	class Core : public galaxy::core::State
	{
	public:
		Core();
		virtual ~Core() = default;

		void on_push() override;
		void on_pop() override;

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;
	};
} // namespace sc

#endif