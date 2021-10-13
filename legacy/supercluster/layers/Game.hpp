///
/// Game.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_INSTANCES_GAME_HPP_
#define SUPERCLUSTER_INSTANCES_GAME_HPP_

#include <galaxy/core/Application.hpp>

using namespace galaxy;

namespace sc
{
	class Game final : public core::Layer
	{
	public:
		Game();
		virtual ~Game();

		void events() override;
		void update() override;
		void pre_render() override;
		void render() override;
	};
} // namespace sc

#endif