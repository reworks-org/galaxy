///
/// AudioPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include <galaxy/audio/Context.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/resource/MusicBook.hpp>
#include <galaxy/resource/SoundBook.hpp>
#include <galaxy/scripting/JSONUtils.hpp>

#include "AudioPanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void AudioPanel::render()
		{
			auto* openal = SL_HANDLE.openal();
			ImGui::Begin("Audio Panel");
			ImGui::Text("OpenAL Context Panel");
			ImGui::Spacing();

			static float s_listener_factor = openal->get_dopper_factor();
			if (ImGui::SliderFloat("Doppler Factor", &s_listener_factor, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_doppler_factor(s_listener_factor);
			}

			static float s_listener_gain = openal->get_listener_gain();
			if (ImGui::SliderFloat("Global Volume", &s_listener_gain, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_gain(s_listener_gain);
			}

			static float s_listener_sos = openal->get_speed_of_sound();
			if (ImGui::SliderFloat("Speed of Sound", &s_listener_sos, 0.1f, 1000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_speed_of_sound(s_listener_sos);
			}

			static glm::vec3 s_listener_pos = openal->get_listener_position();
			ImGui::Text("Listener Position");

			if (ImGui::SliderFloat("X##VEC301", &s_listener_pos.x, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(s_listener_pos);
			}

			if (ImGui::SliderFloat("Y##VEC301", &s_listener_pos.y, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(s_listener_pos);
			}

			if (ImGui::SliderFloat("Z##VEC301", &s_listener_pos.z, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(s_listener_pos);
			}

			ImGui::Spacing();

			static glm::vec3 s_listener_vel = openal->get_listener_velocity();
			ImGui::Text("Listener Velocity");

			if (ImGui::SliderFloat("X##VEC302", &s_listener_vel.x, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(s_listener_vel);
			}

			if (ImGui::SliderFloat("Y##VEC302", &s_listener_vel.y, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(s_listener_vel);
			}

			if (ImGui::SliderFloat("Z##VEC302", &s_listener_vel.z, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(s_listener_vel);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			static bool s_confirmation = false;

			if (ImGui::Button("Add new Music"))
			{
				const auto path = SL_HANDLE.vfs()->show_open_dialog("*.ogg");
				if (path != std::nullopt)
				{
					auto* music = SL_HANDLE.musicbook()->create(std::filesystem::path(path.value()).stem().string());
					music->load(path.value());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to create Music File.");
				}

				s_confirmation = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Add new Sound"))
			{
				const auto path = SL_HANDLE.vfs()->show_open_dialog("*.ogg");
				if (path != std::nullopt)
				{
					auto* sound = SL_HANDLE.soundbook()->create(std::filesystem::path(path.value()).stem().string());
					sound->load(path.value());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to create Sound File.");
				}

				s_confirmation = false;
			}

			ImGui::SameLine();

			if (ImGui::Button("Serialize"))
			{
				auto path = SL_HANDLE.vfs()->absolute("musicbook.json");
				if (path != std::nullopt)
				{
					s_confirmation = json::save_to_disk(path.value(), SL_HANDLE.musicbook()->serialize());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to find musicbook.json.");
				}

				path = SL_HANDLE.vfs()->absolute("soundbook.json");
				if (path != std::nullopt)
				{
					s_confirmation = json::save_to_disk(path.value(), SL_HANDLE.soundbook()->serialize());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to find soundbook.json.");
				}
			}

			if (s_confirmation)
			{
				ImGui::SameLine();
				ImGui::Text("Success!");
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::BeginTabBar("AudioPanelTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable))
			{
				if (ImGui::BeginTabItem("Music", NULL, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton))
				{
					static std::string s_selected         = "Select Music...";
					static audio::Music* s_selected_music = nullptr;
					if (ImGui::BeginCombo("Music", s_selected.c_str()))
					{
						for (auto& [name, music] : SL_HANDLE.musicbook()->cache())
						{
							const bool selected = (s_selected == name);
							if (ImGui::Selectable(name.c_str(), selected))
							{
								s_selected       = name;
								s_selected_music = SL_HANDLE.musicbook()->get(s_selected);
							}

							if (selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}

					if (s_selected != "Select Music...")
					{
						const auto state = s_selected_music->get_state();
						switch (state)
						{
							case AL_PLAYING:
								ImGui::Text("State: PLAYING");
								break;

							case AL_PAUSED:
								ImGui::Text("State: PAUSED");
								break;

							case AL_STOPPED:
								ImGui::Text("State: STOPPED");
								break;

							default:
								ImGui::Text("State: INITIAL");
								break;
						};

						if (ImGui::Button("Play"))
						{
							s_selected_music->play();
						}

						ImGui::SameLine();

						if (ImGui::Button("Pause"))
						{
							s_selected_music->pause();
						}

						ImGui::SameLine();

						if (ImGui::Button("Stop"))
						{
							s_selected_music->stop();
						}

						static bool s_looping = s_selected_music->get_looping();
						if (ImGui::Checkbox("Looping", &s_looping))
						{
							s_selected_music->set_looping(s_looping);
						}

						static float s_pitch = s_selected_music->get_pitch();
						if (ImGui::SliderFloat("Pitch", &s_pitch, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_pitch(s_pitch);
						}

						static float s_gain = s_selected_music->get_gain();
						if (ImGui::SliderFloat("Gain", &s_gain, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_gain(s_gain);
						}

						static float s_rolloff_factor = s_selected_music->get_rolloff_factor();
						if (ImGui::SliderFloat("Rolloff", &s_rolloff_factor, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_rolloff_factor(s_rolloff_factor);
						}

						static float s_max = s_selected_music->get_max_distance();
						if (ImGui::InputFloat(
								"Max Distance", &s_max, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_music->set_max_distance(s_max);
						}

						static glm::vec3 s_cone = s_selected_music->get_cone();
						if (ImGui::SliderFloat("Outer Gain", &s_cone.x, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						if (ImGui::SliderFloat("Inner Gain", &s_cone.y, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						if (ImGui::SliderFloat("Outer Angle", &s_cone.z, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_music->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						static float s_pos[3] = {s_selected_music->get_position().x, s_selected_music->get_position().y, s_selected_music->get_position().z};
						if (ImGui::InputFloat3("Position", s_pos, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_music->set_position({s_pos[0], s_pos[1], s_pos[2]});
						}

						static float s_vel[3] = {s_selected_music->get_velocity().x, s_selected_music->get_velocity().y, s_selected_music->get_velocity().z};
						if (ImGui::InputFloat3("Velocity", s_vel, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_music->set_velocity({s_vel[0], s_vel[1], s_vel[2]});
						}

						static float s_dir[3] = {s_selected_music->get_direction().x, s_selected_music->get_direction().y, s_selected_music->get_direction().z};
						if (ImGui::InputFloat3("Direction", s_dir, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_music->set_direction({s_dir[0], s_dir[1], s_dir[2]});
						}
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Sounds", NULL, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton))
				{
					static std::string s_selected       = "Select Sound...";
					static audio::Sound* s_selected_sfx = nullptr;
					if (ImGui::BeginCombo("Sounds", s_selected.c_str()))
					{
						for (auto& [name, sfx] : SL_HANDLE.soundbook()->cache())
						{
							const bool selected = (s_selected == name);
							if (ImGui::Selectable(name.c_str(), selected))
							{
								s_selected     = name;
								s_selected_sfx = SL_HANDLE.soundbook()->get(s_selected);
							}

							if (selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}

					if (s_selected != "Select Sound...")
					{
						const auto state = s_selected_sfx->get_state();
						switch (state)
						{
							case AL_PLAYING:
								ImGui::Text("State: PLAYING");
								break;

							case AL_PAUSED:
								ImGui::Text("State: PAUSED");
								break;

							case AL_STOPPED:
								ImGui::Text("State: STOPPED");
								break;

							default:
								ImGui::Text("State: INITIAL");
								break;
						};

						if (ImGui::Button("Play"))
						{
							s_selected_sfx->play();
						}

						ImGui::SameLine();

						if (ImGui::Button("Pause"))
						{
							s_selected_sfx->pause();
						}

						ImGui::SameLine();

						if (ImGui::Button("Stop"))
						{
							s_selected_sfx->stop();
						}

						static bool s_looping = s_selected_sfx->get_looping();
						if (ImGui::Checkbox("Looping", &s_looping))
						{
							s_selected_sfx->set_looping(s_looping);
						}

						static float s_pitch = s_selected_sfx->get_pitch();
						if (ImGui::SliderFloat("Pitch", &s_pitch, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_pitch(s_pitch);
						}

						static float s_gain = s_selected_sfx->get_gain();
						if (ImGui::SliderFloat("Gain", &s_gain, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_gain(s_gain);
						}

						static float s_rolloff_factor = s_selected_sfx->get_rolloff_factor();
						if (ImGui::SliderFloat("Rolloff", &s_rolloff_factor, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_rolloff_factor(s_rolloff_factor);
						}

						static float s_max = s_selected_sfx->get_max_distance();
						if (ImGui::InputFloat(
								"Max Distance", &s_max, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_sfx->set_max_distance(s_max);
						}

						static glm::vec3 s_cone = s_selected_sfx->get_cone();
						if (ImGui::SliderFloat("Outer Gain", &s_cone.x, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						if (ImGui::SliderFloat("Inner Gain", &s_cone.y, 0.0f, 1.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						if (ImGui::SliderFloat("Outer Angle", &s_cone.z, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
						{
							s_selected_sfx->set_cone(s_cone.x, s_cone.y, s_cone.z);
						}

						static float s_pos[3] = {s_selected_sfx->get_position().x, s_selected_sfx->get_position().y, s_selected_sfx->get_position().z};
						if (ImGui::InputFloat3("Position", s_pos, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_sfx->set_position({s_pos[0], s_pos[1], s_pos[2]});
						}

						static float s_vel[3] = {s_selected_sfx->get_velocity().x, s_selected_sfx->get_velocity().y, s_selected_sfx->get_velocity().z};
						if (ImGui::InputFloat3("Velocity", s_vel, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_sfx->set_velocity({s_vel[0], s_vel[1], s_vel[2]});
						}

						static float s_dir[3] = {s_selected_sfx->get_direction().x, s_selected_sfx->get_direction().y, s_selected_sfx->get_direction().z};
						if (ImGui::InputFloat3("Direction", s_dir, "%.1f", ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
						{
							s_selected_sfx->set_direction({s_dir[0], s_dir[1], s_dir[2]});
						}
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc