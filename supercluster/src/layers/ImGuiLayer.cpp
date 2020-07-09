///
/// ImGuiLayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "ImGuiLayer.hpp"

///
/// Core namespace.
///
namespace sc
{
	ImGuiLayer::ImGuiLayer() noexcept
		:m_showEUI(false), m_showTEUI(false), m_showTAEUI(false), m_newlyAdded(false), m_drawConsole(false), m_isEntityEnabled(false), m_world(nullptr), m_window(nullptr), m_textureAtlas(nullptr), m_fileToOpen(nullptr), m_fileToSave(nullptr)
	{
		setName("ImGuiLayer");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();

		m_window = SL_HANDLE.window();
		m_world = SL_HANDLE.world();
		ImGui_ImplGlfw_InitForOpenGL(m_window->getGLWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		m_textureAtlas = std::make_unique<qs::TextureAtlas>(4096);
	}

	ImGuiLayer::~ImGuiLayer() noexcept
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::events() noexcept
	{
	}

	void ImGuiLayer::update(protostar::ProtectedDouble* dt) noexcept
	{
	}

	void ImGuiLayer::render(qs::Camera& camera) noexcept
	{
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


	void ImGuiLayer::entityUI() noexcept
	{
		static sr::Entity s_activeE = 0;
		static std::string s_dn = "Enter debug name...";
		static std::string s_activeDN = "No entity selected...";

		ImGui::Begin("Entity Manager", &m_showEUI, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (ImGui::InputText("Create.", &s_dn, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!s_dn.empty())
			{
				s_activeE = m_world->create();
				m_world->assignName(s_activeE, s_dn);
				s_activeDN = s_dn;
				s_dn = "";
			}
			else
			{
				ImGui::SameLine();
				ImGui::Text("Could not create. Empty debug name.");
			}
		}

		if (ImGui::InputText("Create From JSON.", &s_dn, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!s_dn.empty())
			{
				auto path = this->openFilePath();
				s_activeE = m_world->createFromJSON(path.string());
				m_world->assignName(s_activeE, s_dn);
				s_activeDN = s_dn;
				s_dn = "";
			}
			else
			{
				ImGui::SameLine();
				ImGui::Text("Could not create. Empty debug name.");
			}
		}

		if (m_world->validate(s_activeE) && m_world->has(s_activeE))
		{
			std::string out = "Entity numeric id: " + std::to_string(s_activeE);
			ImGui::Text(out.c_str());
		}

		if (ImGui::BeginCombo("Select Entity", s_activeDN.c_str()))
		{
			for (auto& name : m_world->getAllNames())
			{
				const bool selected = (s_activeDN == name.first);
				if (ImGui::Selectable(name.first.c_str(), selected))
				{
					s_activeDN = name.first;
					s_activeE = m_world->findFromName(s_activeDN);
				}

				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (m_world->validate(s_activeE) && m_world->has(s_activeE))
		{
			if (ImGui::Checkbox("Is enabled", &m_isEntityEnabled))
			{
				if (m_isEntityEnabled)
				{
					m_world->add<galaxy::EnabledFlag>(s_activeE);
					m_newlyAdded = true;
				}
				else
				{
					m_world->remove<galaxy::EnabledFlag>(s_activeE);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Serialize - WIP"))
			{
				//m_world->serialize(s_activeE);
				//ImGui::OpenPopup("Entity successfully serialized.");
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Add SpriteComponent"))
			{
				m_world->add<galaxy::SpriteComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add TransformComponent"))
			{
				m_world->add<galaxy::TransformComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add ShaderComponent"))
			{
				m_world->add<galaxy::ShaderComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add SpriteBatchComponent"))
			{
				m_world->add<galaxy::SpriteBatchComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add AudioComponent"))
			{
				m_world->add<galaxy::AudioComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();

			if (ImGui::Button("Add PlaylistComponent"))
			{
				m_world->add<galaxy::PlaylistComponent>(s_activeE);
				m_newlyAdded = true;
			}

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Separator();

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (m_showEUI)
		{
			if (m_world->validate(s_activeE) && m_world->has(s_activeE))
			{
				componentUI(s_activeE);
			}
		}

		ImGui::End();
	}

	void ImGuiLayer::componentUI(sr::Entity active) noexcept
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
			shc = m_world->get<galaxy::ShaderComponent>(active);
		}
		
		if (m_isEntityEnabled)
		{
			ImGui::Separator();
			ImGui::Text("Entity is currently enabled.");

			ImGui::Spacing();
			ImGui::Spacing();
		}
		else
		{
			ImGui::Separator();
			ImGui::Text("Entity is currently disabled.");

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (sc != nullptr)
		{
			ImGui::Separator();
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

		if (tc != nullptr)
		{
			ImGui::Separator();
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

		if (shc != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("Shader Component - WIP");

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (sbc != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("SpriteBatch Component - WIP");

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (pc != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("Playlist Component");

			static std::vector<std::string> s_plf;
			if (ImGui::Button("Load - Can load multiple"))
			{
				auto path = this->openFilePath();
				s_plf.push_back(path.string());
			}

			ImGui::SameLine();

			if (ImGui::Button("Create"))
			{
				pc->m_playlist.load(s_plf);
				s_plf.clear();
			}

			if (ImGui::Button("Play"))
			{
				pc->m_playlist.play();
			}

			if (ImGui::Button("Pause"))
			{
				pc->m_playlist.pause();
			}

			if (ImGui::Button("Stop"))
			{
				pc->m_playlist.stop();
			}

			if (ImGui::Button("Rewind"))
			{
				pc->m_playlist.rewind();
			}

			static bool s_tpl = false;
			if (ImGui::Button("Toggle Looping"), &s_tpl)
			{
				pc->m_playlist.setLooping(s_tpl);
			}

			ImGui::SameLine();

			if (s_tpl)
			{
				ImGui::Text("Playlist is looping.");
			}
			else
			{
				ImGui::Text("Playlist is not looping.");
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}

		if (ac != nullptr)
		{
			ImGui::Separator();
			ImGui::Text("Audio Component");

			if (ImGui::Button("Load"))
			{
				auto path = this->openFilePath();
				ac->m_audio.load(path.string());
			}

			if (ImGui::Button("Play"))
			{
				ac->m_audio.play();
			}

			if (ImGui::Button("Pause"))
			{
				ac->m_audio.pause();
			}

			if (ImGui::Button("Stop"))
			{
				ac->m_audio.stop();
			}

			if (ImGui::Button("Rewind"))
			{
				ac->m_audio.rewind();
			}

			static bool s_tl = false;
			if (ImGui::Button("Toggle Looping"), &s_tl)
			{
				ac->m_audio.setLooping(s_tl);
			}
			
			ImGui::SameLine();

			if (s_tl)
			{
				ImGui::Text("Audio is looping.");
			}
			else
			{
				ImGui::Text("Audio is not looping.");
			}

			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::Separator();
	}

	void ImGuiLayer::scriptEditorUI() noexcept
	{
		const auto cpos = m_editor.GetCursorPosition();

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
				{
					m_editor.SetReadOnly(ro);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo()))
				{
					m_editor.Undo();
				}

				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo()))
				{
					m_editor.Redo();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_editor.HasSelection()))
				{
					m_editor.Copy();
				}

				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Cut();
				}

				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Delete();
				}

				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				{
					m_editor.Paste();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
				{
					m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
				{
					m_editor.SetPalette(TextEditor::GetDarkPalette());
				}

				if (ImGui::MenuItem("Light palette"))
				{
					m_editor.SetPalette(TextEditor::GetLightPalette());
				}

				if (ImGui::MenuItem("Retro blue palette"))
				{
					m_editor.SetPalette(TextEditor::GetRetroBluePalette());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_editor.Render("Script Editor");

		ImGui::End();
	}

	void ImGuiLayer::textureAtlasEditor() noexcept
	{
		static bool s_created = false;
		static bool s_loaded = false;
		static bool s_custom = false;
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
			m_textureAtlas->create(*SL_HANDLE.renderer(), *SL_HANDLE.shaderbook()->get("render_to_texture"));
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

		if (s_loaded && s_created)
		{
			if (ImGui::Button("Add custom Quad"))
			{
				s_custom = !s_custom;
			}
		}

		if (s_custom)
		{
			static float s_x = 0.0f;
			static float s_y = 0.0f;
			static float s_w = 0.0f;
			static float s_h = 0.0f;
			static std::string s_id = "";
			ImGui::Spacing();

			ImGui::InputText("ID", &s_id);
			ImGui::SameLine();
			ImGui::InputFloat("X", &s_x);
			ImGui::SameLine();
			ImGui::InputFloat("Y", &s_y);
			ImGui::SameLine();
			ImGui::InputFloat("Width", &s_w);
			ImGui::SameLine();
			ImGui::InputFloat("Height", &s_h);

			ImGui::Spacing();
			if (ImGui::Button("Add"))
			{
				m_textureAtlas->defineCustomQuad(s_id, { s_x, s_y, s_w, s_h });

				ImGui::Text("Added custom Quad.");
				s_x = 0.0f;
				s_y = 0.0f;
				s_w = 0.0f;
				s_h = 0.0f;
				s_id = "";
			}
		}

		ImGui::End();
	}

	void ImGuiLayer::start() noexcept
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end() noexcept
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	std::filesystem::path ImGuiLayer::openFilePath() noexcept
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

		std::filesystem::path path;
		if (!m_fileToOpen->result().empty())
		{
			path = std::filesystem::path(m_fileToOpen->result()[0]);
			m_fileToOpen.reset();
			m_fileToOpen = nullptr;
		}
		else
		{
			path = "";
		}

		return std::move(path);
	}

	std::filesystem::path ImGuiLayer::saveFilePath() noexcept
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

		std::filesystem::path path;
		if (!m_fileToSave->result().empty())
		{
			path = std::filesystem::path(m_fileToSave->result());
			m_fileToSave.reset();
			m_fileToSave = nullptr;
		}
		else
		{
			path = "";
		}

		return std::move(path);
	}
}