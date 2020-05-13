///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <memory>

#include <qs/core/Shader.hpp>
#include <qs/graphics/Camera.hpp>
#include <protostar/state/State.hpp>

namespace pfd
{
	class open_file;
}

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
		void entityUI() noexcept;
		void componentUI() noexcept;

		void start() noexcept;
		void end() noexcept;

	private:
		bool m_showEUI;
		bool m_showCUI;
		bool m_isFileOpen;

		qs::Window* m_window;
		galaxy::World* m_world;
		std::string m_name;
		std::unique_ptr<pfd::open_file> m_currentOpenFile;
		qs::Shader m_spriteShader;
		qs::Camera m_camera;
	};
}

#endif