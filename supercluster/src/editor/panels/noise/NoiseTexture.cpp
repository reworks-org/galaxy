#include <array>
#include <cstdio>
#include <fstream>
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#include <FastNoise/Metadata.h>

#include <galaxy/ui/ImGuiHelpers.hpp>

#include "NoiseTexture.h"
#include "ImGuiExtra.h"

NoiseTexture::NoiseTexture()
{
	mBuildData.iteration      = 0;
	mBuildData.frequency      = 0.02f;
	mBuildData.seed           = 1337;
	mBuildData.size           = {-1, -1};
	mBuildData.offset         = {};
	mBuildData.generationType = GenType_2D;

	mExportBuildData.size = {4096, 4096};

	for (size_t i = 0; i < 2; i++)
	{
		mThreads.emplace_back(GenerateLoopThread, std::ref(mGenerateQueue), std::ref(mCompleteQueue));
	}

	SetupSettingsHandlers();
}

NoiseTexture::~NoiseTexture()
{
	for (auto& thread : mThreads)
	{
		mGenerateQueue.KillThreads();
		thread.join();
	}

	if (mExportThread.joinable())
	{
		mExportThread.join();
	}
}

void NoiseTexture::Draw(bool* show)
{
	TextureData texData;
	if (mCompleteQueue.Pop(texData))
	{
		if (mCurrentIteration < texData.iteration)
		{
			mNoiseTexture.load_raw(texData.size.x, texData.size.y, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, texData.textureData.data());
			mCurrentIteration = texData.iteration;
		}
	}

	ImGui::SetNextWindowSize(ImVec2(768, 768), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(1143, 305), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Noise Texture Preview", show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::PushItemWidth(82.0f);
		bool edited = false;

		edited |= ImGui::Combo("Generation Type", reinterpret_cast<int*>(&mBuildData.generationType), GenTypeStrings);
		edited |= ImGuiExtra::ScrollCombo(reinterpret_cast<int*>(&mBuildData.generationType), GenType_Count);

		ImVec2 contentSize = ImGui::GetContentRegionAvail();
		ImGui::SameLine();

		glm::ivec2 texSize = {mBuildData.size.x, mBuildData.size.y};

		if (ImGui::DragInt2("Size", glm::value_ptr(texSize), 2, 4, 8192))
		{
			auto d = texSize - mBuildData.size;
			ImVec2 delta(d.x, d.y);

			ImVec2 windowSize = ImGui::GetWindowSize();

			windowSize += delta;
			contentSize += delta;

			ImGui::SetWindowSize(windowSize);
		}
		ImGui::SameLine();

		edited |= ImGui::DragInt("Seed", &mBuildData.seed);
		ImGui::SameLine();

		edited |= ImGui::DragFloat("Frequency", &mBuildData.frequency, 0.001f);
		ImGui::SameLine();

		if (mBuildData.generator && ImGui::Button("Export BMP"))
		{
			auto size             = mExportBuildData.size;
			mExportBuildData      = mBuildData;
			mExportBuildData.size = size;
			ImGui::OpenPopup("Export BMP");
		}

		ImGui::PopItemWidth();

		if (contentSize.x >= 1 && contentSize.y >= 1 && (edited || mBuildData.size.x != (int)contentSize.x || mBuildData.size.y != (int)contentSize.y))
		{
			glm::ivec2 newSize = {(int)contentSize.x, (int)contentSize.y};

			glm::vec2 calc = newSize - mBuildData.size;

			mBuildData.offset.x -= calc.x / 2;
			mBuildData.offset.y -= calc.y / 2;

			mBuildData.size = newSize;
			ReGenerate(mBuildData.generator);
		}

		if (edited)
		{
			ImGuiExtra::MarkSettingsDirty();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);

		galaxy::ui::imgui_imagebutton(mNoiseTexture, {mNoiseTexture.get_widthf(), mNoiseTexture.get_heightf()});
		ImGui::PopStyleColor(3);

		if (ImGui::IsItemHovered())
		{
			glm::vec4 oldOffset = mBuildData.offset;

			if (mBuildData.generationType != GenType_2DTiled && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				auto dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);

				mBuildData.offset.x -= dragDelta.x;
				mBuildData.offset.y += dragDelta.y;
			}
			else if ((mBuildData.generationType == GenType_3D || mBuildData.generationType == GenType_4D) && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
			{
				auto dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);

				mBuildData.offset.z -= dragDelta.x;

				if (mBuildData.generationType == GenType_4D)
				{
					mBuildData.offset.w -= dragDelta.y;
				}
			}

			if (oldOffset != mBuildData.offset)
			{
				ReGenerate(mBuildData.generator);
			}
		}

		DoExport();
	}
	ImGui::End();
}

void NoiseTexture::DoExport()
{
	if (ImGui::BeginPopupModal("Export BMP", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::PushItemWidth(82.0f);
		if (ImGui::DragInt2("Size", glm::value_ptr(mExportBuildData.size), 2, 4, 8192 * 4))
		{
			ImGuiExtra::MarkSettingsDirty();
		}

		if (ImGui::Button("Export (async)"))
		{
			ImGui::CloseCurrentPopup();

			float relativeScale = (float)(mExportBuildData.size.x + mExportBuildData.size.y) / (float)(mBuildData.size.x + mBuildData.size.y);

			mExportBuildData.frequency /= relativeScale;
			mExportBuildData.offset *= relativeScale;

			if (mExportThread.joinable())
			{
				mExportThread.join();
			}
			mExportThread = std::thread([buildData = mExportBuildData]() {
				auto data = BuildTexture(buildData);

				const char* nodeName = buildData.generator->GetMetadata().name;
				std::string filename = nodeName;
				filename += ".bmp";

				// Iterate through file names if filename exists
				for (int i = 1; i < 1024; i++)
				{
					if (!std::filesystem::exists(filename.c_str()))
					{
						break;
					}
					filename = nodeName;
					filename += '_' + std::to_string(i) + ".bmp";
				}

				std::ofstream file(filename.c_str(), std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

				if (file.is_open())
				{
					struct BmpHeader
					{
						// File header (14)
						// char b = 'B';
						// char m = 'M';
						uint32_t fileSize;
						uint32_t reserved   = 0;
						uint32_t dataOffset = 14u + 12u + (256u * 3u);
						// Bmp Info Header (12)
						uint32_t headerSize = 12u;
						uint16_t sizeX;
						uint16_t sizeY;
						uint16_t colorPlanes = 1u;
						uint16_t bitDepth    = 8u;
					};

					int paddedSizeX = buildData.size.x;
					int padding     = paddedSizeX % 4;
					if (padding)
					{
						padding = 4 - padding;
						paddedSizeX += padding;
					}

					BmpHeader header;
					header.fileSize = header.dataOffset + (uint32_t)(paddedSizeX * buildData.size.y);
					header.sizeX    = (uint16_t)buildData.size.x;
					header.sizeY    = (uint16_t)buildData.size.y;

					file << 'B' << 'M';
					file.write(reinterpret_cast<char*>(&header), sizeof(BmpHeader));

					// Colour map
					for (int i = 0; i < 256; i++)
					{
						glm::u8vec2 b3((uint8_t)i);
						file.write(reinterpret_cast<char*>(glm::value_ptr(b3)), 3);
					}

					int xIdx = padding ? buildData.size.x : 0;

					for (uint32_t pix : data.textureData)
					{
						file.write(reinterpret_cast<char*>(&pix), 1);

						if (--xIdx == 0)
						{
							xIdx = buildData.size.x;

							glm::u8vec2 b3(0);
							file.write(reinterpret_cast<char*>(glm::value_ptr(b3)), padding);
						}
					}

					file.close();
				}
			});
		}

		ImGui::PopItemWidth();
		ImGui::EndPopup();
	}
}

void NoiseTexture::ReGenerate(FastNoise::SmartNodeArg<> generator)
{
	mBuildData.generator = generator;
	mBuildData.iteration++;

	mGenerateQueue.Clear();

	if (mBuildData.size.x <= 0 || mBuildData.size.y <= 0)
	{
		return;
	}

	if (generator)
	{
		mGenerateQueue.Push(mBuildData);
		return;
	}

	std::array<uint32_t, 16 * 16> blankTex = {};

	mNoiseTexture.load_raw(16, 16, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, blankTex.data());

	mCurrentIteration = mBuildData.iteration;
}

NoiseTexture::TextureData NoiseTexture::BuildTexture(const BuildData& buildData)
{
	thread_local std::vector<float> noiseData;
	noiseData.resize((size_t)buildData.size.x * buildData.size.y);

	auto gen = FastNoise::New<FastNoise::ConvertRGBA8>(buildData.generator->GetSIMDLevel());
	gen->SetSource(buildData.generator);

	FastNoise::OutputMinMax minMax;

	switch (buildData.generationType)
	{
		case GenType_2D:
			minMax = gen->GenUniformGrid2D(noiseData.data(),
				(int)buildData.offset.x,
				(int)buildData.offset.y,
				buildData.size.x,
				buildData.size.y,
				buildData.frequency,
				buildData.seed);
			break;

		case GenType_2DTiled:
			minMax = gen->GenTileable2D(noiseData.data(), buildData.size.x, buildData.size.y, buildData.frequency, buildData.seed);
			break;

		case GenType_3D:
			minMax = gen->GenUniformGrid3D(noiseData.data(),
				(int)buildData.offset.x,
				(int)buildData.offset.y,
				(int)buildData.offset.z,
				buildData.size.x,
				buildData.size.y,
				1,
				buildData.frequency,
				buildData.seed);
			break;

		case GenType_4D:
			minMax = gen->GenUniformGrid4D(noiseData.data(),
				(int)buildData.offset.x,
				(int)buildData.offset.y,
				(int)buildData.offset.z,
				(int)buildData.offset.w,
				buildData.size.x,
				buildData.size.y,
				1,
				1,
				buildData.frequency,
				buildData.seed);
			break;
		case GenType_Count:
			break;
	}

	return TextureData(buildData.iteration, buildData.size, minMax, noiseData);
}

void NoiseTexture::GenerateLoopThread(GenerateQueue<BuildData>& generateQueue, CompleteQueue<TextureData>& completeQueue)
{
	while (true)
	{
		BuildData buildData = generateQueue.Pop();

		if (generateQueue.ShouldKillThread())
		{
			return;
		}

		TextureData texData = BuildTexture(buildData);
		completeQueue.Push(texData);
	}
}

void NoiseTexture::SetupSettingsHandlers()
{
	ImGuiSettingsHandler editorSettings;
	editorSettings.TypeName   = "NoiseToolNoiseTexture";
	editorSettings.TypeHash   = ImHashStr(editorSettings.TypeName);
	editorSettings.UserData   = this;
	editorSettings.WriteAllFn = [](ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* outBuf) {
		auto* noiseTexture = (NoiseTexture*)handler->UserData;
		outBuf->appendf("\n[%s][Settings]\n", handler->TypeName);

		outBuf->appendf("frequency=%f\n", noiseTexture->mBuildData.frequency);
		outBuf->appendf("seed=%d\n", noiseTexture->mBuildData.seed);
		outBuf->appendf("gen_type=%d\n", (int)noiseTexture->mBuildData.generationType);
		outBuf->appendf("export_size=%d:%d\n", noiseTexture->mExportBuildData.size.x, noiseTexture->mExportBuildData.size.y);
	};
	editorSettings.ReadOpenFn = [](ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name) -> void* {
		if (strcmp(name, "Settings") == 0)
		{
			return handler->UserData;
		}

		return nullptr;
	};
	editorSettings.ReadLineFn = [](ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line) {
		auto* noiseTexture = (NoiseTexture*)handler->UserData;

		sscanf(line, "frequency=%f", &noiseTexture->mBuildData.frequency);
		sscanf(line, "seed=%d", &noiseTexture->mBuildData.seed);
		sscanf(line, "gen_type=%d", (int*)&noiseTexture->mBuildData.generationType);
		sscanf(line, "export_size=%d:%d", &noiseTexture->mExportBuildData.size.x, &noiseTexture->mExportBuildData.size.y);
	};

	ImGuiExtra::AddOrReplaceSettingsHandler(editorSettings);
}
