///
/// Game.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_GAME_HPP_
#define SUPERCLUSTER_INSTANCES_GAME_HPP_

#include <galaxy/core/Instance.hpp>

using namespace galaxy;

namespace sc
{
	class Game final : public core::Instance
	{
	public:
		Game();
		virtual ~Game();

		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;
	};
} // namespace sc

#endif