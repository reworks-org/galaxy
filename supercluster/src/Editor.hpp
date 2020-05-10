///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <protostar/state/State.hpp>

namespace sc
{
	class Editor : public protostar::State
	{
	public:
		Editor() noexcept;
		~Editor() noexcept override;
		void onPush() noexcept override;
		void onPop() noexcept override;
		void events() noexcept override;
		void update(protostar::ProtectedDouble* deltaTime) noexcept override;
		void render() noexcept override;

	private:

	};
}

#endif