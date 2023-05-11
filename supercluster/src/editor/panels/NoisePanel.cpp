///
/// NoisePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "NoisePanel.hpp"

namespace sc
{
	namespace panel
	{
		NoisePanel::NoisePanel()
			: m_max_simd_level {0}
			, m_level_names {"Auto"}
			, m_level_enums {FastSIMD::Level_Null}
		{
			m_node_editor = std::make_unique<FastNoiseNodeEditor>();

			for (int i = 1; i > 0; i <<= 1)
			{
				const auto level = static_cast<FastSIMD::eLevel>(i);
				if (level & FastNoise::SUPPORTED_SIMD_LEVELS & FastSIMD::COMPILED_SIMD_LEVELS)
				{
					m_level_names.emplace_back(FastNoiseNodeEditor::GetSIMDLevelName(level));
					m_level_enums.emplace_back(level);
				}
			}
		}

		NoisePanel::~NoisePanel()
		{
			m_node_editor.reset();
		}

		void NoisePanel::render(bool* show, UpdateStack& updates)
		{
			m_node_editor->Draw(show);

			if (ImGui::Begin("NoiseGraphMisc", show))
			{
				const auto size = static_cast<int>(m_level_enums.size());

				if (ImGui::Combo("Max SIMD Level", &m_max_simd_level, m_level_names.data(), size) || ImGuiExtra::ScrollCombo(&m_max_simd_level, size))
				{
					m_node_editor->SetSIMDLevel(m_level_enums[m_max_simd_level]);
				}

				ImGui::SameLine();

				if (ImGui::Button("RESET GRAPH"))
				{
					updates.push_back([&]() {
						m_node_editor.reset();
						m_node_editor = std::make_unique<FastNoiseNodeEditor>();
					});
				}

				m_node_editor->DrawStats();
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc