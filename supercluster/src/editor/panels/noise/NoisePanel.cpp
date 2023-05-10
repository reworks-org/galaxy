///
/// NoisePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_stdlib.h>

#include "ImGuiExtra.h"

#include "NoisePanel.hpp"

namespace sc
{
	namespace panel
	{
		NoisePanel::NoisePanel()
		{
			mNodeEditor = std::make_unique<FastNoiseNodeEditor>();

			mLevelNames = {"Auto"};
			mLevelEnums = {FastSIMD::Level_Null};

			for (int i = 1; i > 0; i <<= 1)
			{
				FastSIMD::eLevel lvl = (FastSIMD::eLevel)i;
				if (lvl & FastNoise::SUPPORTED_SIMD_LEVELS & FastSIMD::COMPILED_SIMD_LEVELS)
				{
					mLevelNames.emplace_back(FastNoiseNodeEditor::GetSIMDLevelName(lvl));
					mLevelEnums.emplace_back(lvl);
				}
			}
		}

		NoisePanel::~NoisePanel()
		{
			mNodeEditor.reset();
		}

		void NoisePanel::render(bool* show, UpdateStack& updates)
		{
			mNodeEditor->Draw(show);

			if (ImGui::Begin("NoiseGraphMisc", show))
			{
				if (ImGui::Combo("Max SIMD Level", &mMaxSIMDLevel, mLevelNames.data(), (int)mLevelEnums.size()) ||
					ImGuiExtra::ScrollCombo(&mMaxSIMDLevel, (int)mLevelEnums.size()))
				{
					FastSIMD::eLevel newLevel = mLevelEnums[mMaxSIMDLevel];
					mNodeEditor->SetSIMDLevel(newLevel);
				}

				ImGui::SameLine();

				if (ImGui::Button("RESET GRAPH"))
				{
					updates.push_back([&]() {
						mNodeEditor.reset();
						mNodeEditor = std::make_unique<FastNoiseNodeEditor>();
					});
				}

				mNodeEditor->DrawStats();
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc