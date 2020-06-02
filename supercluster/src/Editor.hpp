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
#include <qs/graphics/TextureAtlas.hpp>

namespace pfd
{
	class open_file;
	class save_file;
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
		void componentUI(sr::Entity active) noexcept;
		void scriptEditorUI() noexcept;
		void textureAtlasEditor() noexcept;

		void start() noexcept;
		void end() noexcept;

	private:
		bool m_showEUI;
		bool m_showTEUI;
		bool m_showTAEUI;
		bool m_isFileOpen;
		bool m_newlyAdded;

		std::string m_name;
		galaxy::World* m_world;

		qs::Camera m_camera;
		qs::Window* m_window;
		qs::Shader m_spriteShader;
		qs::Shader m_atlasShader;
		std::unique_ptr<qs::TextureAtlas> m_textureAtlas;

		TextEditor m_editor;
		std::unique_ptr<pfd::open_file> m_fileToOpen;
		std::unique_ptr<pfd::save_file> m_fileToSave;

		galaxy::SpriteComponent* sc = nullptr;
		galaxy::TransformComponent* tc = nullptr;
		galaxy::SpriteBatchComponent* sbc = nullptr;
		galaxy::PlaylistComponent* pc = nullptr;
		galaxy::AudioComponent* ac = nullptr;
	};
}

#endif