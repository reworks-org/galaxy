#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <FastNoise/FastNoise.h>
#include <FastNoise/Metadata.h>
#include <FastNoise/MultiThreadQueues.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_addons/imnodes.h>
#include <imgui_addons/material_design_icons.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <galaxy/graphics/Texture.hpp>

using namespace galaxy;

namespace ImGuiExtra
{
	bool ScrollCombo(int* comboIndex, int comboCount);
} // namespace ImGuiExtra

namespace sc
{
	// Adapted from
	// https://github.com/Auburn/FastNoise2/tree/master/NoiseTool
	class NoiseTexture
	{
	public:
		enum GenType
		{
			GenType_2D,
			GenType_2DTiled,
			GenType_3D,
			GenType_4D,
			GenType_Count
		};

		inline static const char* GenTypeStrings = "2D\0"
												   "2D Tiled\0"
												   "3D Slice\0"
												   "4D Slice\0";

		NoiseTexture();
		~NoiseTexture();

		void Draw(bool* show);
		void ReGenerate(FastNoise::SmartNodeArg<> generator);

	private:
		struct BuildData
		{
			FastNoise::SmartNode<const FastNoise::Generator> generator;
			glm::ivec2 size;
			glm::vec4 offset;
			float frequency;
			int32_t seed;
			uint64_t iteration;
			GenType generationType;
		};

		struct TextureData
		{
			TextureData() = default;

			TextureData(uint64_t iter, glm::ivec2 s, FastNoise::OutputMinMax mm, const std::vector<float>& v)
				: minMax(mm)
				, size(s)
				, iteration(iter)
				, textureData(v)
			{
			}

			std::vector<float> textureData;
			FastNoise::OutputMinMax minMax;
			glm::ivec2 size;
			uint64_t iteration;
		};

		static TextureData BuildTexture(const BuildData& buildData);
		static void GenerateLoopThread(GenerateQueue<BuildData>& generateQueue, CompleteQueue<TextureData>& completeQueue);

		void DoExport();

		graphics::Texture mNoiseTexture;
		uint64_t mCurrentIteration = 0;

		BuildData mBuildData;
		BuildData mExportBuildData;
		FastNoise::OutputMinMax mMinMax;

		std::vector<std::thread> mThreads;
		GenerateQueue<BuildData> mGenerateQueue;
		CompleteQueue<TextureData> mCompleteQueue;
		bool mPopupControl = false;
	};
} // namespace sc