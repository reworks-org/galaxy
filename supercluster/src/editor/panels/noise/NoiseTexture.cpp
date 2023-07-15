#include <glm/gtc/type_ptr.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "NoiseTexture.h"

namespace ImGuiExtra
{
	bool ScrollCombo(int* comboIndex, int comboCount)
	{
		if (ImGui::IsItemHovered())
		{
			if (ImGui::GetIO().MouseWheel < 0 && *comboIndex < comboCount - 1)
			{
				(*comboIndex)++;
				return true;
			}

			if (ImGui::GetIO().MouseWheel > 0 && *comboIndex > 0)
			{
				(*comboIndex)--;
				return true;
			}
		}
		return false;
	}
} // namespace ImGuiExtra

namespace sc
{
	NoiseTexture::NoiseTexture()
	{
		mBuildData.iteration      = 0;
		mBuildData.frequency      = 0.02f;
		mBuildData.seed           = 1337;
		mBuildData.size           = {-1, -1};
		mBuildData.offset         = {};
		mBuildData.generationType = GenType_2D;

		mExportBuildData.size = {512, 512};

		for (size_t i = 0; i < 2; i++)
		{
			mThreads.emplace_back(GenerateLoopThread, std::ref(mGenerateQueue), std::ref(mCompleteQueue));
		}
	}

	NoiseTexture::~NoiseTexture()
	{
		for (auto& thread : mThreads)
		{
			mGenerateQueue.KillThreads();
			thread.join();
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
		if (ImGui::Begin("Graph Preview", show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
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
				auto delta = texSize - mBuildData.size;

				ImVec2 windowSize = ImGui::GetWindowSize();

				windowSize.x += delta.x;
				windowSize.y += delta.y;

				contentSize.x += delta.x;
				contentSize.y += delta.y;

				ImGui::SetWindowSize(windowSize);
			}
			ImGui::SameLine();

			edited |= ImGui::DragInt("Seed", &mBuildData.seed);
			ImGui::SameLine();

			edited |= ImGui::DragFloat("Frequency", &mBuildData.frequency, 0.001f);
			ImGui::SameLine();

			if (mBuildData.generator && ImGui::Button("Export Texture"))
			{
				auto size             = mExportBuildData.size;
				mExportBuildData      = mBuildData;
				mExportBuildData.size = size;
				ImGui::OpenPopup("Exporter");
				mPopupControl = true;
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

			ImGui::PushStyleColor(ImGuiCol_Button, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0);

			ui::imgui_imagebutton(mNoiseTexture, {mNoiseTexture.get_widthf(), mNoiseTexture.get_heightf()});
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
		if (ImGui::BeginPopupModal("Exporter", &mPopupControl, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::TextUnformatted("Warning: Larger the size, slower this will run!");
			ImGui::DragInt2("Size", glm::value_ptr(mExportBuildData.size), 2, 4, 8192 * 4);

			if (ImGui::Button("Run"))
			{
				const auto relativeScale = (float)(mExportBuildData.size.x + mExportBuildData.size.y) / (float)(mBuildData.size.x + mBuildData.size.y);
				mExportBuildData.frequency /= relativeScale;
				mExportBuildData.offset *= relativeScale;

				auto data = BuildTexture(mExportBuildData);
				mNoiseTexture.load_raw(data.size.x, data.size.y, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, data.textureData.data());

				std::string nodeName = mExportBuildData.generator->GetMetadata().name;
				nodeName += ".png";

				auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
				const auto path = fs.open_save_dialog(nodeName, {"*.png"});

				if (!path.empty())
				{
					mNoiseTexture.save(path);
				}

				ImGui::CloseCurrentPopup();
			}

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
} // namespace sc