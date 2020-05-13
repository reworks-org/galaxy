///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <qs/core/Window.hpp>
#include <imgui_impl_opengl3.h>
#include <galaxy/core/World.hpp>
#include <qs/shaders/Sprites.hpp>
#include <solar/entity/Manager.hpp>
#include <pfd/portable-file-dialogs.h>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/scripting/JSONDefinition.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
		:m_showEUI(false), m_showCUI(false), m_showTEUI(false), m_isFileOpen(false), m_name("Editor"), m_window(nullptr), m_world(nullptr), m_currentOpenFile(nullptr)
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
		m_camera.create(0.0f, m_window->getWidth(), m_window->getHeight(), 0.0f);
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

			if (ImGui::MenuItem("Script Editor"))
			{
				m_showTEUI = !m_showTEUI;
			}

			if (ImGui::BeginMenu("ECS"))
			{
				if (ImGui::MenuItem("Entity Manager"))
				{
					m_showEUI = !m_showEUI;
				}

				if (ImGui::MenuItem("Component Manager"))
				{
					m_showCUI = !m_showCUI;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (m_showEUI)
		{
			entityUI();
		}

		if (m_showCUI)
		{
			componentUI();
		}

		if (m_showTEUI)
		{
			scriptEditorUI();
		}

		end();
	}

	void Editor::entityUI() noexcept
	{
		static std::filesystem::path path = "";
		ImGui::Begin("Entities", &m_showEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Open Definition"))
		{
			if (!m_isFileOpen)
			{
				m_isFileOpen = true;
				m_currentOpenFile = std::make_unique<pfd::open_file>("Open entity JSON definition.");
			}
		}

		if ((m_isFileOpen && m_currentOpenFile->ready()) && (!m_currentOpenFile->result().empty()))
		{
			path = std::filesystem::path(m_currentOpenFile->result()[0]);
			m_isFileOpen = false;
			m_currentOpenFile.reset();
			m_currentOpenFile = nullptr;
		}

		if (!path.empty())
		{
			SL_HANDLE.world()->createFromJSON(path.string());
			path = "";
		}
		
		ImGui::End();
	}
	
	void Editor::componentUI() noexcept
	{
		ImGui::Begin("Entities", &m_showCUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::End();
	}

	void Editor::scriptEditorUI() noexcept
	{
		static std::filesystem::path path = "";
		static const std::array<const char*, 2> filter = {
				"*.json", "*.lua"
		};

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
						m_currentOpenFile = std::make_unique<pfd::open_file>("Open entity JSON definition.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					//m_editor.GetCursorPosition();
					//auto text = m_editor.GetText();
					//std::ofstream out(m_currentFile, std::ios::out | std::ios::trunc);
					//out << text;
					//out.close();
				}

				if (ImGui::MenuItem("Close"))
				{
					m_showTEUI = false;
					m_editor.SetText("");
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

		if ((m_isFileOpen && m_currentOpenFile->ready()) && (!m_currentOpenFile->result().empty()))
		{
			path = std::filesystem::path(m_currentOpenFile->result()[0]);
			m_isFileOpen = false;
			m_currentOpenFile.reset();
			m_currentOpenFile = nullptr;
		}

		if (!path.empty())
		{
			if (path.extension() == ".json")
			{
				m_editor.SetLanguageDefinition(galaxy::getJsonDefinition());
			}
			else if (path.extension() == ".lua")
			{
				m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
			}

			std::ifstream text(path.string(), std::ifstream::in);
			if (!text.fail())
			{
				std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
				m_editor.SetText(str);
			}
			else
			{
				PL_LOG(PL_ERROR, "Failed to read file: " + path.string());
			}

			text.close();
			path = "";
		}
			
		m_editor.Render("Script Editor");

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