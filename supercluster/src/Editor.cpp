///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <imgui_impl_glfw.h>
#include <qs/core/Window.hpp>
#include <imgui_impl_opengl3.h>
#include <galaxy/core/World.hpp>
#include <qs/shaders/Sprites.hpp>
#include <solar/entity/Manager.hpp>
#include <pfd/portable-file-dialogs.h>
#include <qs/shaders/RenderToTexture.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/scripting/JSONDefinition.hpp>

#include <galaxy/components/SpriteComponent.hpp>
#include <galaxy/components/TransformComponent.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
		:m_showEUI(false), m_showTEUI(false), m_showTAEUI(false), m_isFileOpen(false), m_world(nullptr), m_window(nullptr), m_textureAtlas(nullptr), m_fileToOpen(nullptr), m_fileToSave(nullptr)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();

		m_window = SL_HANDLE.window();
		m_world = SL_HANDLE.world();
		ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		m_spriteShader.loadFromRaw(qs::s_spriteVS, qs::s_spriteFS);
		m_atlasShader.loadFromRaw(qs::s_renderToTextureVS, qs::s_renderToTextureFS);

		m_camera.create(0.0f, m_window->getWidth(), m_window->getHeight(), 0.0f);
		m_textureAtlas = std::make_unique<qs::TextureAtlas>(4096);
	}

	Editor::~Editor() noexcept
	{
	}

	void Editor::onPush() noexcept
	{
	}

	void Editor::onPop() noexcept
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		m_window = nullptr;
	}

	void Editor::events() noexcept
	{
		if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			m_window->close();
		}

		m_world->events();
	}

	void Editor::update(protostar::ProtectedDouble* deltaTime) noexcept
	{
		m_world->update(deltaTime);
		m_camera.update(deltaTime->get());
	}

	void Editor::render() noexcept
	{
		m_spriteShader.bind();
		m_spriteShader.setUniform<glm::mat4>("u_cameraProj", m_camera.getProj());
		m_spriteShader.setUniform<glm::mat4>("u_cameraView", m_camera.getTransformation());
		
		m_world->get<galaxy::RenderSystem>()->render(m_world, m_spriteShader);

		start();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					m_window->close();
				}

				if (ImGui::MenuItem("Restart"))
				{
					SL_HANDLE.m_restart = true;
					m_window->close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editors"))
			{
				if (ImGui::MenuItem("Script Editor"))
				{
					m_showTEUI = !m_showTEUI;
				}

				if (ImGui::MenuItem("Texture Atlas Editor"))
				{
					m_showTAEUI = !m_showTAEUI;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("ECS"))
			{
				if (ImGui::MenuItem("Entity Manager"))
				{
					m_showEUI = !m_showEUI;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (m_showEUI)
		{
			entityUI();
		}

		if (m_showTEUI)
		{
			scriptEditorUI();
		}

		if (m_showTAEUI)
		{
			textureAtlasEditor();
		}

		end();
	}

	void Editor::entityUI() noexcept
	{
		static std::filesystem::path path = "";
		static sr::Entity active = 0;
		ImGui::Begin("Entities", &m_showEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Create from JSON"))
		{
			if (!m_isFileOpen)
			{
				m_isFileOpen = true;
				m_fileToOpen = std::make_unique<pfd::open_file>("Open entity JSON definition.");
			}
		}

		if (ImGui::Button("Create Entity"))
		{
			active = m_world->create();
		}

		if ((m_isFileOpen && m_fileToOpen->ready()) && (!m_fileToOpen->result().empty()))
		{
			path = std::filesystem::path(m_fileToOpen->result()[0]);
			active = m_world->createFromJSON(path.string());
			
			m_isFileOpen = false;
			m_fileToOpen.reset();
			m_fileToOpen = nullptr;
			path = "";
		}

		if (m_world->validate(active))
		{
			componentUI(active);
		}

		ImGui::End();
	}
	
	void Editor::componentUI(sr::Entity active) noexcept
	{
		ImGui::Separator();

		auto tuple = m_world->multi<galaxy::SpriteComponent, galaxy::TransformComponent>(active);

		auto sc = std::get<0>(tuple);
		if (sc != nullptr)
		{
			ImGui::Text("Sprite Component");

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Load texture"))
			{
				//sc->load("");
				//sc->create<qs::BufferTypeDynamic>();
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Clamp to Border"))
			{
				sc->clampToBorder(protostar::Black);
			}

			if (ImGui::Button("Clamp to Edge"))
			{
				sc->clampToEdge();
			}

			if (ImGui::Button("Set Mirrored"))
			{
				sc->setMirrored();
			}

			if (ImGui::Button("Set Repeated"))
			{
				sc->setRepeated();
			}

			ImGui::Spacing();
			ImGui::Spacing();

			static int ansio = 0;
			if (ImGui::SliderInt("Set Ansiotrophy", &ansio, 1, 4))
			{
				sc->setAnisotropy(ansio * 2);
			}

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Set Minify to Nearest"))
			{
				sc->setMinifyFilter(qs::TextureFilter::NEAREST);
			}

			if (ImGui::Button("Set Minify to Linear"))
			{
				sc->setMinifyFilter(qs::TextureFilter::LINEAR);
			}

			if (ImGui::Button("Set Magnify to Nearest"))
			{
				sc->setMagnifyFilter(qs::TextureFilter::NEAREST);
			}

			if (ImGui::Button("Set Magnify to Linear"))
			{
				sc->setMagnifyFilter(qs::TextureFilter::LINEAR);
			}
		}

		ImGui::Separator();
	}

	void Editor::scriptEditorUI() noexcept
	{
		auto cpos = m_editor.GetCursorPosition();

		ImGui::Begin("Script Editor", &m_showTEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					if (!m_isFileOpen)
					{
						m_isFileOpen = true;
						m_fileToOpen = std::make_unique<pfd::open_file>("Open entity JSON definition.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					if (!m_isFileOpen)
					{
						m_isFileOpen = true;
						m_fileToSave = std::make_unique<pfd::save_file>("Save script.");
					}
				}

				if (ImGui::MenuItem("Close"))
				{
					m_showTEUI = false;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = m_editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					m_editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
					m_editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
					m_editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
					m_editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_editor.HasSelection()))
					m_editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
					m_editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					m_editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					m_editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					m_editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					m_editor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (m_fileToSave && m_isFileOpen)
		{
			if (m_fileToSave->ready() && !m_fileToSave->result().empty())
			{
				auto text = m_editor.GetText();
				auto wp = std::filesystem::path(m_fileToSave->result());

				std::ofstream out(wp.string(), std::ios::out | std::ios::trunc);
				out << text;
				out.close();

				m_fileToSave.reset();
				m_fileToSave = nullptr;
				m_isFileOpen = false;
			}
		}

		if (m_fileToOpen && m_isFileOpen)
		{
			if (m_fileToOpen->ready() && !m_fileToOpen->result().empty())
			{
				auto fp = std::filesystem::path(m_fileToOpen->result()[0]);
				if (fp.extension() == ".json")
				{
					m_editor.SetLanguageDefinition(galaxy::getJsonDefinition());
				}
				else if (fp.extension() == ".lua")
				{
					m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
				}

				std::ifstream text(fp.string(), std::ifstream::in);
				if (!text.fail())
				{
					std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
					m_editor.SetText(str);
				}
				else
				{
					PL_LOG(PL_ERROR, "Failed to read file: " + fp.string());
				}

				text.close();

				m_isFileOpen = false;
				m_fileToOpen.reset();
				m_fileToOpen = nullptr;
			}
		}
			
		m_editor.Render("Script Editor");

		ImGui::End();
	}

	void Editor::textureAtlasEditor() noexcept
	{
		static bool add = false;
		static std::unique_ptr<pfd::open_file> path = nullptr;
		ImGui::Begin("Texture Atlas Editor", &m_showTAEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Create from JSON"))
		{
			if (!add)
			{
				add = true;
				path = std::make_unique<pfd::open_file>("Select *.json definition.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Texture"))
		{
			if (!add)
			{
				add = true;
				path = std::make_unique<pfd::open_file>("Select *.png texture.");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Create"))
		{
			m_textureAtlas->create(*SL_HANDLE.window(), *SL_HANDLE.renderer(), m_atlasShader);
		}

		if (add == true && path != nullptr)
		{
			if (path->ready() && !path->result().empty())
			{
				auto fp = std::filesystem::path(path->result()[0]);
				if (fp.extension() == ".json")
				{
					nlohmann::json root;
					std::ifstream ifs(fp.string(), std::ifstream::in);
					ifs >> root;
					ifs.close();

					auto array = root.at("textures");

					std::for_each(array.begin(), array.end(), [&](const nlohmann::json& texture)
					{
						m_textureAtlas->add(texture);
					});
				}
				else
				{
					m_textureAtlas->add(fp.string());
				}

				add = false;
				path.reset();
				path = nullptr;
			}
		}

		ImGui::End();
	}

	void Editor::start() noexcept
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Editor::end() noexcept
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}