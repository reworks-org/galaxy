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
#include <galaxy/components/SpriteBatchComponent.hpp>
#include <galaxy/components/PlaylistComponent.hpp>
#include <galaxy/components/AudioComponent.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
		:m_showEUI(false), m_showTEUI(false), m_showTAEUI(false), m_newlyAdded(false), m_drawConsole(false), m_world(nullptr), m_window(nullptr), m_textureAtlas(nullptr), m_fileToOpen(nullptr), m_fileToSave(nullptr)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();

		m_window = SL_HANDLE.window();
		m_world = SL_HANDLE.world();
		ImGui_ImplGlfw_InitForOpenGL(m_window->getGLWindow(), true);
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
		if (glfwGetKey(m_window->getGLWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

			if (ImGui::MenuItem("Lua Console"))
			{
				m_drawConsole = !m_drawConsole;
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

		if (m_drawConsole)
		{
			m_console.draw(&m_drawConsole);
		}

		end();
	}

	void Editor::entityUI() noexcept
	{
		static sr::Entity active = 0;
		ImGui::Begin("Entities", &m_showEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Create from JSON"))
		{
			auto path = this->openFilePath();
			active = m_world->createFromJSON(path.string());
		}

		if (ImGui::Button("Create Entity"))
		{
			active = m_world->create();
		}

		static std::string s_aenText = "";
		if (ImGui::InputText("Active Entity Name", &s_aenText, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (m_listOfEntitys.size() <= active)
			{
				m_listOfEntitys.resize(active);
			}

			m_listOfEntitys[active] = s_aenText;
		}
		
		static int s_index = 0;
		if (ImGui::Combo("Selected Entity", &s_index, m_listOfEntitys))
		{
			//sr::Entity entity = free << 16 | sr::VALID_ENTITY;
		}


		if (m_world->validate(active))
		{
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Add SpriteComponent"))
			{
				m_world->add<galaxy::SpriteComponent>(active);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add TransformComponent"))
			{
				m_world->add<galaxy::TransformComponent>(active);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add SpriteBatchComponent"))
			{
				m_world->add<galaxy::SpriteBatchComponent>(active);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add AudioComponent"))
			{
				m_world->add<galaxy::AudioComponent>(active);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add PlaylistComponent"))
			{
				m_world->add<galaxy::PlaylistComponent>(active);
				m_newlyAdded = true;
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (m_world->validate(active))
		{
			componentUI(active);
		}

		ImGui::End();
	}
	
	void Editor::componentUI(sr::Entity active) noexcept
	{
		static sr::Entity curEntity = 0;
		if (curEntity != active || m_newlyAdded == true)
		{
			curEntity = active;
			m_newlyAdded = false;

			sc = m_world->get<galaxy::SpriteComponent>(active);
			tc = m_world->get<galaxy::TransformComponent>(active);
			sbc = m_world->get<galaxy::SpriteBatchComponent>(active);
			pc = m_world->get<galaxy::PlaylistComponent>(active);
			ac = m_world->get<galaxy::AudioComponent>(active);
		}

		ImGui::Separator();

		if (sc != nullptr)
		{
			ImGui::Text("Sprite Component");

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Load texture"))
			{
				auto path = this->openFilePath();

				sc->load(path.string());
				sc->create<qs::BufferTypeDynamic>();
			}

			if (ImGui::Button("Clamp to Border"))
			{
				sc->clampToBorder();
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

			static int s_ansio = 0;
			if (ImGui::SliderInt("Set Ansiotrophy", &s_ansio, 1, 4))
			{
				sc->setAnisotropy(s_ansio * 2);
			}

			static float s_scOpacity = 1.0f;
			if (ImGui::SliderFloat("Opacity", &s_scOpacity, 0.0f, 1.0f))
			{
				sc->setOpacity(s_scOpacity);
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

		if (tc != nullptr)
		{
			ImGui::Text("Transform Component");

			ImGui::Spacing();
			ImGui::Spacing();

			static float s_xy[2] = { 0.0f, 0.0f };
			if (ImGui::SliderFloat2("Move", &s_xy[0], 0.0f, 100.0f))
			{
				tc->m_transform.setPos(s_xy[0], s_xy[1]);
			}

			static float r = 0.0f;
			if (ImGui::SliderAngle("Rotate", &r))
			{
				tc->m_transform.rotate(r);
			}

			static float scale = 1.0f;
			if (ImGui::SliderFloat("Scale", &scale, 0.0f, 2.0f))
			{
				tc->m_transform.scale(scale);
			}
		}

		ImGui::Separator();

		if (sbc != nullptr)
		{
			ImGui::Text("SpriteBatch Component");

			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::Separator();

		if (pc != nullptr)
		{
			ImGui::Text("Playlist Component");

			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::Separator();

		if (ac != nullptr)
		{
			ImGui::Text("Audio Component");

			ImGui::Spacing();
			ImGui::Spacing();
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
					auto fp = this->openFilePath();
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
				}

				if (ImGui::MenuItem("Save"))
				{
					auto fp = this->saveFilePath();
					auto text = m_editor.GetText();

					std::ofstream out(fp.string(), std::ios::out | std::ios::trunc);
					out << text;
					out.close();
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
			
		m_editor.Render("Script Editor");

		ImGui::End();
	}

	void Editor::textureAtlasEditor() noexcept
	{
		static bool s_created = false;
		static bool s_loaded = false;
		ImGui::Begin("Texture Atlas Editor", &m_showTAEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::Button("Create from JSON"))
		{
			auto fp = this->openFilePath();
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
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Texture"))
		{
			auto fp = this->openFilePath();
			m_textureAtlas->add(fp.string());
		}

		ImGui::SameLine();

		if (ImGui::Button("Create"))
		{
			m_textureAtlas->create(*SL_HANDLE.renderer(), m_atlasShader);
			s_created = true;
		}

		if (s_loaded)
		{
			ImGui::Text("Loaded texture(s).");
		}

		if (s_created)
		{
			ImGui::Text("Texture Atlas created successfully!");
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

	std::filesystem::path Editor::openFilePath() noexcept
	{
		m_fileToOpen = std::make_unique<pfd::open_file>("Open file.");

		if (m_fileToOpen->ready())
		{
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);

			std::condition_variable cv;
			cv.wait(lock, [&]()
			{
				return (!m_fileToOpen->result().empty());
			});
		}

		auto path = std::filesystem::path(m_fileToOpen->result()[0]);
		m_fileToOpen.reset();
		m_fileToOpen = nullptr;

		return std::move(path);
	}

	std::filesystem::path Editor::saveFilePath() noexcept
	{
		m_fileToSave = std::make_unique<pfd::save_file>("Save file.");

		if (m_fileToSave->ready())
		{
			std::mutex mutex;
			std::unique_lock<std::mutex> lock(mutex);

			std::condition_variable cv;
			cv.wait(lock, [&]()
				{
					return (!m_fileToSave->result().empty());
				});
		}

		auto path = std::filesystem::path(m_fileToSave->result());
		m_fileToSave.reset();
		m_fileToSave = nullptr;

		return std::move(path);
	}
}