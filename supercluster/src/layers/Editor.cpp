///
/// Editor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/math/ZLib.hpp>
#include <galaxy/scripting/JSONUtils.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "Editor.hpp"

namespace sc
{
	Editor::Editor() noexcept
		: m_current_project_path {""}
	{
		GALAXY_LOG_CAPTURE_CUSTOM(m_std_console.get_stream());
		set_name("Editor");

		//m_framebuffer.create(1, 1);
		//editor::theme::visual_dark();
		//m_entity_panel.set_layer(this);

		//m_checkerboard.load_mem(tex::checkerboard);
		//m_checkerboard.set_minify_filter<graphics::NearestMipmapFilter>();
		//m_checkerboard.set_magnify_filter<graphics::NearestTexFilter>();
		//m_checkerboard.set_mode<graphics::TextureModes::REPEAT>();
	}

	Editor::~Editor()
	{
		GALAXY_LOG_CAPTURE_CUSTOM(std::cout);
		m_scene_stack.clear();
	}

	void Editor::events()
	{
		/*
		if (!m_game_mode)
		{
			//	ImGui::ImplGlfw::g_BlockInput = false;

			if (m_viewport_focused && m_viewport_hovered)
			{
				if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				}
				else
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
				}

				const auto& camera = m_scene_stack.top()->m_camera;
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
				{
					m_imgui_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

					m_mousemoved_event.m_x += m_imgui_delta.x;
					m_mousemoved_event.m_y += m_imgui_delta.y;

					//camera.on_event(m_mousemoved_event);
					ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
				}

				m_scroll_delta.m_x_offset = 0.0;
				m_scroll_delta.m_y_offset = SL_HANDLE.window()->get_scroll_delta();
				if (m_scroll_delta.m_y_offset != 0.0)
				{
					//camera.on_event(m_scroll_delta);
				}

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					m_mouse_picked = true;
				}

				if (SL_HANDLE.window()->key_down(input::Keys::W))
				{
					m_keydown_event.m_keycode = input::Keys::W;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::W;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::S))
				{
					m_keydown_event.m_keycode = input::Keys::S;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::S;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::A))
				{
					m_keydown_event.m_keycode = input::Keys::A;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::A;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::D))
				{
					m_keydown_event.m_keycode = input::Keys::D;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::D;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::Q))
				{
					m_keydown_event.m_keycode = input::Keys::Q;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::Q;
					//camera.on_event(m_keyup_event);
				}

				if (SL_HANDLE.window()->key_down(input::Keys::E))
				{
					m_keydown_event.m_keycode = input::Keys::E;
					//camera.on_event(m_keydown_event);
				}
				else
				{
					m_keyup_event.m_keycode = input::Keys::E;
					//camera.on_event(m_keyup_event);
				}
			}
		}
		else
		{
			//ImGui::ImplGlfw::g_BlockInput = true;
			m_scene_stack.top()->on_push();

			if (SL_HANDLE.window()->key_pressed(input::Keys::ESC))
			{
				m_game_mode = false;
				deserialize(m_backup);
				SL_HANDLE.window()->set_cursor_visibility(true);
			}

			m_scene_stack.events();
		}
		*/
	}

	void Editor::update()
	{
		/*
		if (!m_paused)
		{
			m_scene_stack.update();
		}
		*/
	}

	void Editor::pre_render()
	{
		/*
		if (!m_game_mode)
		{
			for (const auto& gl_operation : m_gl_operations)
			{
				gl_operation();
			}

			m_gl_operations.clear();
			m_scene_stack.pre_render();

			m_framebuffer.bind(true);
			m_scene_stack.render();
			m_framebuffer.unbind();

			imgui_render();
		}
		else
		{
			m_scene_stack.pre_render();
		}
		*/
	}

	void Editor::render()
	{
		/*
		if (!m_game_mode)
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else
		{
			m_scene_stack.render();
		}
		*/
	}

	void Editor::new_project()
	{
		SL_HANDLE.window()->set_title("Untitled Project - Supercluster Editor");
		m_scene_stack.clear();
		m_scene_stack.create("New Scene");
		m_scene_stack.push("New Scene");
	}

	void Editor::load_project(std::string_view path)
	{
		const auto compressed = SL_HANDLE.vfs()->open_binary(path);
		if (compressed != std::nullopt)
		{
			const auto fs_path     = std::filesystem::path(path);
			m_current_project_path = fs_path.string();

			auto decompressed = math::decode_zlib({compressed.value().begin(), compressed.value().end()});
			auto json         = json::parse_from_mem(decompressed);
			if (json != std::nullopt)
			{
				deserialize(json.value());
				SL_HANDLE.window()->set_title(std::format("{0} - Supercluster Editor", fs_path.stem().string()));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse json from memory after decompression for: {0}.", fs_path.string());
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to load project file.");
		}
	}

	void Editor::save_project()
	{
		if (m_current_project_path.empty())
		{
			const auto sp_opt = SL_HANDLE.vfs()->show_save_dialog("*.scproj", "projects/");
			if (sp_opt != std::nullopt)
			{
				SL_HANDLE.vfs()->create_file(sp_opt.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Project file was not created on disk.");
			}

			auto compressed = math::encode_zlib(serialize().dump(4));
			if (!SL_HANDLE.vfs()->save_binary(compressed, sp_opt.value()))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save file: {0}.", sp_opt.value());
			}

			SL_HANDLE.window()->set_title(std::format("{0} - Supercluster Editor", std::filesystem::path(sp_opt.value()).stem().string()));
		}
		else
		{
			auto compressed = math::encode_zlib(serialize().dump(4));
			if (!SL_HANDLE.vfs()->save_binary(compressed, m_current_project_path))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save file: {0}.", m_current_project_path);
			}

			SL_HANDLE.window()->set_title(std::format("{0} - Supercluster Editor", std::filesystem::path(m_current_project_path).stem().string()));
		}
	}

	/*
	void Editor::exit()
	{
		for (auto* process : m_processes)
		{
			platform::close_process(process);
			process = nullptr;
		}

		SL_HANDLE.window()->close();
	}
	*/

	void Editor::imgui_render()
	{
		ui::imgui_new_frame();

		// clang-format off
		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
		static constexpr const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
		// clang-format on

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Viewport", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ImGui::DockSpace(ImGui::GetID("Main Viewport Dockspace"), {0.0f, 0.0f}, dockspace_flags);

		/*
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("New"))
				{
					pfd::message msg("WARNING", "Save?\nUnsaved work will be lost.", pfd::choice::yes_no, pfd::icon::warning);
					if (msg.result() == pfd::button::yes)
					{
						save();
					}

					new_project();
				}

				if (ImGui::MenuItem("Load"))
				{
					pfd::message msg("WARNING", "Save?\nUnsaved work will be lost.", pfd::choice::yes_no, pfd::icon::warning);
					if (msg.result() == pfd::button::yes)
					{
						save();
					}

					auto file = SL_HANDLE.vfs()->show_open_dialog("*.scproj");
					if (file != std::nullopt)
					{
						load(file.value());
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open project file.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					save();
				}

				if (ImGui::MenuItem("Restart"))
				{
					SL_HANDLE.m_restart = true;
					exit();
				}

				if (ImGui::MenuItem("Exit"))
				{
					exit();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editor"))
			{
				if (ImGui::BeginMenu("Theme"))
				{
					if (ImGui::MenuItem("Light"))
					{
						ImGui::StyleColorsLight();
					}

					if (ImGui::MenuItem("Dark"))
					{
						ImGui::StyleColorsDark();
					}

					if (ImGui::MenuItem("Classic"))
					{
						ImGui::StyleColorsClassic();
					}

					if (ImGui::MenuItem("Enhanced Light"))
					{
						editor::theme::enhanced_light();
					}

					if (ImGui::MenuItem("Enhanced Dark"))
					{
						editor::theme::enhanced_dark();
					}

					if (ImGui::MenuItem("Material Dark"))
					{
						editor::theme::material_dark();
					}

					if (ImGui::MenuItem("Visual Dark"))
					{
						editor::theme::visual_dark();
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Open Tiled"))
				{
					m_processes.push_back(platform::run_process("tools/tiled/tiled.exe"));
				}

				if (ImGui::MenuItem("Open Blender"))
				{
					m_processes.push_back(platform::run_process("tools/blender/blender.exe"));
				}

				if (ImGui::MenuItem("Show ImGui::Demo"))
				{
					m_render_demo = !m_render_demo;
				}

				ImGui::EndMenu();
			}

			ImGui::Text("( ? )");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("[SHIFT] for docking.\n[RMB] to drag viewport.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
			if (ImGui::ArrowButton("PlaySceneArrowButton", ImGuiDir_Right))
			{
				m_game_mode = true;
				m_backup    = serialize();

				SL_HANDLE.window()->set_cursor_visibility(false);
			}

			static std::string s_pause_resume = " || ##PauseSceneButton";
			if (ImGui::Button(s_pause_resume.c_str()))
			{
				if (!m_paused)
				{
					m_paused       = true;
					s_pause_resume = " >> ##ResumeSceneButton";
				}
				else
				{
					m_paused       = false;
					s_pause_resume = " || ##PauseSceneButton";
				}
			}

			ImGui::EndMenuBar();
		}

		m_entity_panel.render(m_gl_operations);
		m_json_panel.parse_and_display();
		m_console.render();
		m_scene_panel.render(m_scene_stack, m_gl_operations);
		m_script_panel.render();
		m_std_console.render();
		m_audio_panel.render();

		viewport();

		if (m_render_demo)
		{
			ImGui::ShowDemoWindow(&m_render_demo);
		}
		*/

		ImGui::End();
	}

	void Editor::viewport()
	{
		/*
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			m_viewport_focused    = ImGui::IsWindowFocused();
			m_viewport_hovered    = ImGui::IsWindowHovered();
			const auto size_avail = ImGui::GetContentRegionAvail();
			if (size_avail != m_viewport_size)
			{
				m_viewport_size = size_avail;
				m_framebuffer.resize(m_viewport_size.x, m_viewport_size.y);
			}

			if (m_topscene_type == "2D")
			{
				ImGui::Image(reinterpret_cast<void*>(m_checkerboard.gl_texture()), m_viewport_size, {0, 1}, {1, 0});
			}

			ImGui::Image(reinterpret_cast<void*>(m_framebuffer.get_texture()), m_viewport_size, {0, 1}, {1, 0});

			if (m_mouse_picked)
			{
				if (m_topscene_type == "2D")
				{
					static constexpr const auto mp_id = std::numeric_limits<ecs::Entity>::max();

					scene::Scene2D* const s2d = static_cast<scene::Scene2D*>(m_scene_stack.top().get());
					const auto& c2d           = s2d->get_camera();

					glm::vec2 pos;
					pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - c2d.get_pos().x;
					pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - c2d.get_pos().y;

					// Will be erased by collision system, as this is after update().
					auto* const tree = s2d->m_world.get_system<systems::CollisionSystem>()->get_tree();
					tree->insert(mp_id, {pos.x, pos.y}, {pos.x + 4, pos.y + 4});

					static std::vector<ecs::Entity> possible = {};
					tree->query(mp_id, std::back_inserter(possible));

					if (possible.size() > 0)
					{
						m_entity_panel.set_selected_entity(std::make_optional(possible[0]));
					}
					else
					{
						m_entity_panel.set_selected_entity(std::nullopt);
					}

					possible.clear();
				}
				else
				{
					// TODO: 3D Picking.
				}

		m_mouse_picked = false;
	}

	if (ImGui::BeginPopupContextItem("RightClickCreateEntityPopup"))
	{
		if (ImGui::BeginMenu("  Create"))
		{
			if (ImGui::BeginMenu("  Entity"))
			{
				if (m_topscene_type == "2D")
				{
					if (ImGui::MenuItem("  Sprite"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::BatchSprite>(entity);
						world.create_component<components::Tag>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::BATCHED;
					}

					if (ImGui::MenuItem("  Animated Sprite"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::Animated>(entity);
						world.create_component<components::BatchSprite>(entity);
						world.create_component<components::Tag>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::BATCHED;
					}

					if (ImGui::MenuItem("  Primitive2D"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::Primitive2D>(entity);
						world.create_component<components::Tag>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::LINE_LOOP;
					}

					if (ImGui::MenuItem("  Rigid Body"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::BatchSprite>(entity);
						world.create_component<components::OnCollision>(entity);
						world.create_component<components::RigidBody>(entity);
						world.create_component<components::Tag>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::BATCHED;
					}

					if (ImGui::MenuItem("  Animated Body"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::Animated>(entity);
						world.create_component<components::BatchSprite>(entity);
						world.create_component<components::OnCollision>(entity);
						world.create_component<components::RigidBody>(entity);
						world.create_component<components::Tag>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::BATCHED;
					}

					if (ImGui::MenuItem("  Text"))
					{
						auto& world       = m_scene_stack.top()->m_world;
						const auto entity = world.create();

						world.create_component<components::Tag>(entity);
						world.create_component<components::Text>(entity);
						world.create_component<components::Transform2D>(entity);
						auto* r   = world.create_component<components::Renderable>(entity);
						r->m_type = graphics::Renderables::TEXT;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
		}

		ImGui::End();
		ImGui::PopStyleVar(1);
		*/
	}
} // namespace sc