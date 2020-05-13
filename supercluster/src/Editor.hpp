///
/// Editor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_HPP_
#define SUPERCLUSTER_EDITOR_HPP_

#include <memory>

#include <TextEditor.h>
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
		void scriptEditorUI() noexcept;

		void start() noexcept;
		void end() noexcept;

	private:
		bool m_showEUI;
		bool m_showCUI;
		bool m_showTEUI;
		bool m_isFileOpen;

		std::string m_name;

		qs::Camera m_camera;
		qs::Window* m_window;
		qs::Shader m_spriteShader;
		
		galaxy::World* m_world;
		
		TextEditor m_editor;
		std::unique_ptr<pfd::open_file> m_currentOpenFile;
	};
}

#endif