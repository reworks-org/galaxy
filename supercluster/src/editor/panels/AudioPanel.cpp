///
/// AudioPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include <galaxy/audio/Context.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "AudioPanel.hpp"

namespace sc
{
	namespace panel
	{
		void AudioPanel::render()
		{
			auto* openal = SL_HANDLE.openal();
			ImGui::Begin("Audio Panel");

			static float s_factor = openal->get_dopper_factor();
			if (ImGui::SliderFloat("Doppler Factor", &s_factor, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_doppler_factor(s_factor);
			}

			static float s_gain = openal->get_listener_gain();
			if (ImGui::SliderFloat("Global Volume", &s_gain, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_gain(s_gain);
			}

			static float s_sos = openal->get_speed_of_sound();
			if (ImGui::SliderFloat("Speed of Sound", &s_sos, 0.1f, 1000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_speed_of_sound(s_sos);
			}

			static glm::vec3 pos = openal->get_listener_position();
			ImGui::Text("Listener Position");

			if (ImGui::SliderFloat("X##VEC301", &pos.x, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(pos);
			}

			if (ImGui::SliderFloat("Y##VEC301", &pos.y, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(pos);
			}

			if (ImGui::SliderFloat("Z##VEC301", &pos.z, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_position(pos);
			}

			ImGui::Spacing();

			static glm::vec3 vel = openal->get_listener_velocity();
			ImGui::Text("Listener Velocity");

			if (ImGui::SliderFloat("X##VEC302", &vel.x, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(vel);
			}

			if (ImGui::SliderFloat("Y##VEC302", &vel.y, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(vel);
			}

			if (ImGui::SliderFloat("Z##VEC302", &vel.z, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_ClampOnInput))
			{
				openal->set_listener_velocity(vel);
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc