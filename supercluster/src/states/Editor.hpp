///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <qs/post/Simple.hpp>
#include <protostar/state/State.hpp>
#include <galaxy/layer/LayerStorage.hpp>

namespace sc
{
	class Editor : public pr::State
	{
	public:
		Editor();
		virtual ~Editor() = default;
		void on_push() override;
		void on_pop() override;
		void events() override;
		void update(const double dt) override;
		void pre_render() override;
		void render() override;

	private:
		qs::effect::Simple m_simple_postrenderer;
		galaxy::LayerStorage m_layers;
	};
} // namespace sc

#endif