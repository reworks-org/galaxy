#pragma once
#include <vector>
#include <memory>
#include <cstring>
#include <span>

#include <FastNoise/FastNoise.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <galaxy/graphics/Texture.hpp>

#include "MultiThreadQueues.h"

using namespace galaxy;

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
	void SetupSettingsHandlers();

	graphics::Texture mNoiseTexture;
	uint64_t mCurrentIteration = 0;

	BuildData mBuildData;
	BuildData mExportBuildData;
	FastNoise::OutputMinMax mMinMax;

	std::vector<std::thread> mThreads;
	GenerateQueue<BuildData> mGenerateQueue;
	CompleteQueue<TextureData> mCompleteQueue;
};