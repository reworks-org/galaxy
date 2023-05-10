#include <sstream>
#include <random>
#include <cstdio>
#include <array>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_stdlib.h>
#include <imgui_addons/imnodes.h>
#include <imgui_addons/material_design_icons.h>

#include <galaxy/ui/ImGuiHelpers.hpp>

#include "ImGuiExtra.h"
#include "FastNoiseNodeEditor.h"
#include "DemoNodeTrees.inl"

static bool MatchingGroup(const std::vector<const char*>& a, const std::vector<const char*>& b)
{
	std::string aString;
	for (const char* c : a)
	{
		aString.append(c);
		aString.push_back('\t');
	}

	std::string bString;
	for (const char* c : b)
	{
		bString.append(c);
		bString.push_back('\t');
	}

	return aString == bString;
}

template<typename T>
static bool MatchingMembers(const std::vector<T>& a, const std::vector<T>& b)
{
	if (a.size() != b.size())
	{
		return false;
	}

	for (size_t i = 0; i < a.size(); i++)
	{
		if (strcmp(a[i].name, b[i].name) != 0)
		{
			return false;
		}
	}
	return true;
}

static std::string TimeWithUnits(int64_t time, int significantDigits = 3)
{
	if (time == 0)
	{
		return "0us";
	}

	double f = time / 1e+3;

	int d = (int)::ceil(::log10(::abs(f))); /*digits before decimal point*/
	std::stringstream ss;
	ss.precision(std::max(significantDigits - d, 0));
	ss << std::fixed << f << "us";
	return ss.str();
}

template<typename... Args>
std::string string_format(const char* format, Args... args)
{
	int size_s = std::snprintf(nullptr, 0, format, args...);
	if (size_s <= 0)
	{
		return "";
	}
	auto size = static_cast<size_t>(size_s);
	std::string buf(size, 0);
	std::snprintf(buf.data(), size, format, args...);
	return buf;
}

template<typename... T>
static bool DoHoverPopup(const char* format, T... args)
{
	if (ImGui::IsItemHovered())
	{
		std::string hoverTxt = string_format(format, args...);

		if (hoverTxt.empty())
		{
			return false;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(hoverTxt.c_str());
		ImGui::EndTooltip();
		ImGui::PopStyleVar();
		return true;
	}
	return false;
}

FastNoiseNodeEditor::Node::Node(FastNoiseNodeEditor& e, FastNoise::NodeData* nodeData, bool generatePreview, int id)
	: Node(e, std::unique_ptr<FastNoise::NodeData>(nodeData), generatePreview, id)
{
}

FastNoiseNodeEditor::Node::Node(FastNoiseNodeEditor& e, std::unique_ptr<FastNoise::NodeData>&& nodeData, bool generatePreview, int id)
	: editor(e)
	, data(std::move(nodeData))
	, nodeId(id ? id : e.GetFreeNodeId())
{
	assert(!e.FindNodeFromId(id));

	if (generatePreview)
	{
		GeneratePreview();
	}
}

void FastNoiseNodeEditor::Node::GeneratePreview(bool nodeTreeChanged, bool benchmark)
{
	static std::array<float, NoiseSize * NoiseSize> noiseData;

	serialised     = FastNoise::Metadata::SerialiseNodeData(data.get(), true);
	auto generator = FastNoise::NewFromEncodedNodeTree(serialised.c_str(), editor.mMaxSIMDLevel);

	if (!benchmark && nodeTreeChanged)
	{
		generateAverages.clear();
	}

	if (generator)
	{
		auto genRGB = FastNoise::New<FastNoise::ConvertRGBA8>(editor.mMaxSIMDLevel);
		genRGB->SetSource(generator);

		auto startTime = std::chrono::high_resolution_clock::now();

		editor.GenerateNodePreviewNoise(genRGB.get(), noiseData.data());

		generateAverages.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() -
								   editor.mOverheadNode.totalGenerateNs);

		std::sort(generateAverages.begin(), generateAverages.end());

		totalGenerateNs = generateAverages[generateAverages.size() / 3];
	}
	else
	{
		std::fill(noiseData.begin(), noiseData.end(), 0.0f);
		serialised.clear();
		totalGenerateNs = 0;
	}

	if (benchmark)
	{
		return;
	}

	noiseTexture.load_raw(NoiseSize, NoiseSize, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, noiseData.data());

	for (auto& node : editor.mNodes)
	{
		for (FastNoise::NodeData* link : node.second.GetNodeIDLinks())
		{
			if (link == data.get())
			{
				node.second.GeneratePreview(nodeTreeChanged);
			}
		}
	}

	if (nodeTreeChanged)
	{
		if (editor.mSelectedNode == data.get())
		{
			editor.ChangeSelectedNode(data.get());
		}
	}
}

std::vector<FastNoise::NodeData*> FastNoiseNodeEditor::Node::GetNodeIDLinks()
{
	std::vector<FastNoise::NodeData*> links;
	links.reserve(data->nodeLookups.size() + data->hybrids.size());

	for (FastNoise::NodeData* link : data->nodeLookups)
	{
		links.emplace_back(link);
	}

	for (auto& link : data->hybrids)
	{
		links.emplace_back(link.first);
	}

	assert(links.size() < 16);

	return links;
}

int64_t FastNoiseNodeEditor::Node::GetLocalGenerateNs()
{
	int64_t localTotal = totalGenerateNs;

	for (FastNoise::NodeData* link : GetNodeIDLinks())
	{
		auto find = editor.mNodes.find(link);

		if (find != editor.mNodes.end())
		{
			localTotal -= find->second.totalGenerateNs;
		}
	}

	return std::max<int64_t>(localTotal, 0);
}

FastNoise::NodeData*& FastNoiseNodeEditor::Node::GetNodeLink(int attributeId)
{
	attributeId &= 15;

	if (attributeId < (int)data->nodeLookups.size())
	{
		return data->nodeLookups[attributeId];
	}
	else
	{
		attributeId -= (int)data->nodeLookups.size();
		return data->hybrids[attributeId].first;
	}
}

void FastNoiseNodeEditor::Node::AutoPositionChildNodes(ImVec2 nodePos, float verticalSpacing)
{
	auto nodeLinks = GetNodeIDLinks();
	nodeLinks.erase(std::remove(nodeLinks.begin(), nodeLinks.end(), nullptr), nodeLinks.end());

	if (nodeLinks.empty())
	{
		return;
	}

	ImVec2 nodeSpacing = {280, verticalSpacing};

	nodePos.x -= nodeSpacing.x;
	nodePos.y -= nodeSpacing.y * 0.5f * (nodeLinks.size() - 1);

	for (FastNoise::NodeData* link : nodeLinks)
	{
		ImNodes::SetNodeScreenSpacePos(editor.mNodes.at(link).nodeId, nodePos);

		editor.mNodes.at(link).AutoPositionChildNodes(nodePos, nodeLinks.size() > 1 ? verticalSpacing * 0.6f : verticalSpacing);
		nodePos.y += nodeSpacing.y;
	}
}

bool FastNoiseNodeEditor::MetadataMenuItem::CanDraw(std::function<bool(const FastNoise::Metadata*)> isValid) const
{
	return !isValid || isValid(metadata);
}

const FastNoise::Metadata* FastNoiseNodeEditor::MetadataMenuItem::DrawUI(std::function<bool(const FastNoise::Metadata*)> isValid, bool drawGroups) const
{
	std::string format = FastNoise::Metadata::FormatMetadataNodeName(metadata, true);

	if (ImGui::MenuItem(format.c_str()))
	{
		return metadata;
	}
	return nullptr;
}

bool FastNoiseNodeEditor::MetadataMenuGroup::CanDraw(std::function<bool(const FastNoise::Metadata*)> isValid) const
{
	for (const auto& item : items)
	{
		if (item->CanDraw(isValid))
		{
			return true;
		}
	}
	return false;
}

const FastNoise::Metadata* FastNoiseNodeEditor::MetadataMenuGroup::DrawUI(std::function<bool(const FastNoise::Metadata*)> isValid, bool drawGroups) const
{
	const FastNoise::Metadata* returnPressed = nullptr;

	bool doGroup = drawGroups && name[0] != 0;

	if (!doGroup || ImGui::BeginMenu(name))
	{
		for (const auto& item : items)
		{
			if (item->CanDraw(isValid))
			{
				if (auto pressed = item->DrawUI(isValid, drawGroups))
				{
					returnPressed = pressed;
				}
			}
		}
		if (doGroup)
		{
			ImGui::EndMenu();
		}
	}
	return returnPressed;
}

FastNoiseNodeEditor::FastNoiseNodeEditor()
	: mOverheadNode(*this, new FastNoise::NodeData(&FastNoise::Metadata::Get<FastNoise::Constant>()), false)
{
	ImNodes::CreateContext();
	ImNodes::GetIO().AltMouseButton = ImGuiMouseButton_Right;

	ImNodes::StyleColorsDark();

	ImNodes::GetStyle().MiniMapPadding = ImVec2(8, 8);

	// Create Metadata context menu tree
	std::unordered_map<std::string, MetadataMenuGroup*> groupMap;
	auto* root = new MetadataMenuGroup("");
	mContextMetadata.emplace_back(root);

	auto menuSort = [](const MetadataMenu* a, const MetadataMenu* b) {
		return std::strcmp(a->GetName(), b->GetName()) < 0;
	};

	for (const FastNoise::Metadata* metadata : FastNoise::Metadata::GetAll())
	{
		auto* metaDataGroup = root;

		std::string groupTree;

		for (const char* group : metadata->groups)
		{
			groupTree += group;
			auto find = groupMap.find(groupTree);
			if (find == groupMap.end())
			{
				auto* newGroup = new MetadataMenuGroup(group);
				mContextMetadata.emplace_back(newGroup);
				metaDataGroup->items.emplace_back(newGroup);
				find = groupMap.emplace(groupTree, newGroup).first;

				std::sort(metaDataGroup->items.begin(), metaDataGroup->items.end(), menuSort);
			}

			metaDataGroup = find->second;
			groupTree += '\t';
		}

		metaDataGroup->items.emplace_back(mContextMetadata.emplace_back(new MetadataMenuItem(metadata)).get());
		std::sort(metaDataGroup->items.begin(), metaDataGroup->items.end(), menuSort);
	}
}

void FastNoiseNodeEditor::Draw(bool* show)
{
	ImGui::SetNextWindowSize(ImVec2(963, 634), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(8, 439), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(ICON_MDI_QRCODE_EDIT " Noise Graph", show))
	{
		UpdateSelected();

		bool edited = false;
		ImGui::PushItemWidth(82.0f);

		edited |= ImGui::Combo("Generation Type", reinterpret_cast<int*>(&mNodeGenType), NoiseTexture::GenTypeStrings);
		edited |= ImGuiExtra::ScrollCombo(reinterpret_cast<int*>(&mNodeGenType), NoiseTexture::GenType_Count);
		ImGui::SameLine();

		edited |= ImGui::DragInt("Seed", &mNodeSeed);
		ImGui::SameLine();
		edited |= ImGui::DragFloat("Frequency", &mNodeFrequency, 0.001f);

		ImGui::PopItemWidth();

		if (edited)
		{
			for (auto& node : mNodes)
			{
				node.second.GeneratePreview(false);
			}
		}

		ImNodes::BeginNodeEditor();

		DoHelp();

		DoContextMenu();

		DoNodes();

		ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomLeft);

#if 0
        if( ImGui::IsWindowHovered() )
        {
            auto zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
            ImNodes::EditorContextSetZoom( zoom, ImGui::GetMousePos() );
        }
#endif

		ImNodes::EndNodeEditor();

		CheckLinks();
	}
	ImGui::End();

	mNoiseTexture.Draw(show);
}

void FastNoiseNodeEditor::DrawStats()
{
	std::string simdTxt = "Current SIMD Level: ";
	simdTxt += GetSIMDLevelName(mActualSIMDLevel);
	ImGui::TextUnformatted(simdTxt.c_str());
}

void FastNoiseNodeEditor::CheckLinks()
{
	// Check for new links
	int startNodeId, endNodeId;
	int startAttr, endAttr;
	bool createdFromSnap;
	if (ImNodes::IsLinkCreated(&startNodeId, &startAttr, &endNodeId, &endAttr, &createdFromSnap))
	{
		Node* startNode = FindNodeFromId(startNodeId);
		Node* endNode   = FindNodeFromId(endNodeId);

		if (startNode && endNode)
		{
			auto& link = endNode->GetNodeLink(endAttr);

			if (!createdFromSnap || !link)
			{
				link = startNode->data.get();
				endNode->GeneratePreview();
			}
		}
	}

	int attrStart;
	if (ImNodes::IsLinkDropped(&attrStart, false))
	{
		Node* node = FindNodeFromId(Node::GetNodeIdFromAttribute(attrStart));

		if (node && (attrStart & Node::AttributeBitMask) == Node::AttributeBitMask)
		{
			mDroppedLinkNode = node->data.get();
			mDroppedLink     = true;
		}
	}
}

void FastNoiseNodeEditor::DeleteNode(FastNoise::NodeData* nodeData)
{
	mNodes.erase(nodeData);

	for (auto& node : mNodes)
	{
		bool changed = false;
		int attrId   = node.second.GetStartingAttributeId();

		for (FastNoise::NodeData* link : node.second.GetNodeIDLinks())
		{
			if (link == nodeData)
			{
				node.second.GetNodeLink(attrId) = nullptr;
				changed                         = true;
			}
			attrId++;
		}

		if (changed)
		{
			node.second.GeneratePreview();
		}
	}
}

void FastNoiseNodeEditor::UpdateSelected()
{
	std::vector<int> linksToDelete;
	int selectedLinkCount = ImNodes::NumSelectedLinks();

	bool delKeyPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete), false) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace), false);

	if (selectedLinkCount && delKeyPressed)
	{
		linksToDelete.resize(selectedLinkCount);
		ImNodes::GetSelectedLinks(linksToDelete.data());
	}

	int destroyedLinkId;
	if (ImNodes::IsLinkDestroyed(&destroyedLinkId))
	{
		linksToDelete.push_back(destroyedLinkId);
	}

	for (int deleteID : linksToDelete)
	{
		for (auto& node : mNodes)
		{
			bool changed    = false;
			int attributeId = node.second.GetStartingAttributeId();

			for (FastNoise::NodeData* link : node.second.GetNodeIDLinks())
			{
				(void)link;
				if (attributeId == deleteID)
				{
					node.second.GetNodeLink(attributeId) = nullptr;
					changed                              = true;
				}
				attributeId++;
			}

			if (changed)
			{
				node.second.GeneratePreview();
			}
		}
	}

	int selectedNodeCount = ImNodes::NumSelectedNodes();

	if (selectedNodeCount && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete), false))
	{
		std::vector<int> selected(selectedNodeCount);

		ImNodes::GetSelectedNodes(selected.data());

		for (int deleteID : selected)
		{
			if (Node* node = FindNodeFromId(deleteID))
			{
				DeleteNode(node->data.get());
			}
		}
	}
}

void FastNoiseNodeEditor::SetSIMDLevel(FastSIMD::eLevel lvl)
{
	mMaxSIMDLevel = lvl;

	mOverheadNode.generateAverages.clear();

	for (auto& node : mNodes)
	{
		node.second.generateAverages.clear();
		node.second.GeneratePreview(false);
	}

	ChangeSelectedNode(mSelectedNode);
}

void FastNoiseNodeEditor::DoNodes()
{
	for (auto& node : mNodes)
	{
		ImNodes::BeginNode(node.second.nodeId);

		ImNodes::BeginNodeTitleBar();
		std::string formatName = FastNoise::Metadata::FormatMetadataNodeName(node.second.data->metadata);
		ImGui::TextUnformatted(formatName.c_str());

#ifdef NDEBUG
		DoHoverPopup("%s", node.first->metadata->description);
#else
		DoHoverPopup("%s : %d\n%s", node.first->metadata->name, node.first->metadata->id, node.first->metadata->description);
#endif
		std::string performanceString = TimeWithUnits(node.second.GetLocalGenerateNs());

		ImGui::SameLine(Node::NoiseSize - ImGui::CalcTextSize(performanceString.c_str()).x);
		ImGui::TextUnformatted(performanceString.c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
			ImGui::SetTooltip("Total: %s", TimeWithUnits(node.second.totalGenerateNs).c_str());
			ImGui::PopStyleVar();
		}

		ImNodes::EndNodeTitleBar();

		// Right click node title to change node type
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy Encoded Node Tree"))
			{
				ImGui::SetClipboardText(node.second.serialised.c_str());
			}

			ImGui::Separator();
			ImGui::MenuItem("Convert To:", nullptr, nullptr, false);

			auto& nodeMetadata = node.second.data->metadata;
			auto newMetadata   = mContextMetadata.front()->DrawUI(
                [nodeMetadata](const FastNoise::Metadata* metadata) {
                    return metadata != nodeMetadata && MatchingGroup(metadata->groups, nodeMetadata->groups);
                },
                false);

			if (newMetadata)
			{
				if (MatchingMembers(newMetadata->memberVariables, nodeMetadata->memberVariables) &&
					MatchingMembers(newMetadata->memberNodeLookups, nodeMetadata->memberNodeLookups) &&
					MatchingMembers(newMetadata->memberHybrids, nodeMetadata->memberHybrids))
				{
					nodeMetadata = newMetadata;
				}
				else
				{
					FastNoise::NodeData newData(newMetadata);

					std::queue<FastNoise::NodeData*> links;

					for (FastNoise::NodeData* link : node.second.data->nodeLookups)
					{
						links.emplace(link);
					}
					for (auto& link : node.second.data->hybrids)
					{
						links.emplace(link.first);
					}

					for (auto& link : newData.nodeLookups)
					{
						if (links.empty())
							break;
						link = links.front();
						links.pop();
					}
					for (auto& link : newData.hybrids)
					{
						if (links.empty())
							break;
						link.first = links.front();
						links.pop();
					}

					*node.second.data = std::move(newData);
				}

				node.second.GeneratePreview();
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		ImGui::PushItemWidth(60.0f);

		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap);
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
		int attributeId    = node.second.GetStartingAttributeId();
		auto& nodeMetadata = node.second.data->metadata;
		auto& nodeData     = node.second.data;

		for (auto& memberNode : nodeMetadata->memberNodeLookups)
		{
			ImNodes::BeginInputAttribute(attributeId++);
			formatName = FastNoise::Metadata::FormatMetadataMemberName(memberNode);
			ImGui::TextUnformatted(formatName.c_str());
			ImNodes::EndInputAttribute();

			DoHoverPopup(memberNode.description);
		}

		for (size_t i = 0; i < node.second.data->metadata->memberHybrids.size(); i++)
		{
			ImNodes::BeginInputAttribute(attributeId++);

			bool isLinked           = node.second.data->hybrids[i].first;
			const char* floatFormat = "%.3f";

			if (isLinked)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				floatFormat = "";
			}

			formatName = FastNoise::Metadata::FormatMetadataMemberName(nodeMetadata->memberHybrids[i]);

			if (ImGui::DragFloat(formatName.c_str(), &nodeData->hybrids[i].second, 0.02f, 0, 0, floatFormat))
			{
				node.second.GeneratePreview();
			}

			if (isLinked)
			{
				ImGui::PopItemFlag();
			}
			ImNodes::EndInputAttribute();
			DoHoverPopup(nodeMetadata->memberHybrids[i].description);
		}

		for (size_t i = 0; i < nodeMetadata->memberVariables.size(); i++)
		{
			ImNodes::BeginStaticAttribute(0);

			auto& nodeVar = nodeMetadata->memberVariables[i];

			formatName = FastNoise::Metadata::FormatMetadataMemberName(nodeVar);

			switch (nodeVar.type)
			{
				case FastNoise::Metadata::MemberVariable::EFloat:
					{
						if (ImGui::DragFloat(formatName.c_str(), &nodeData->variables[i].f, 0.02f, nodeVar.valueMin.f, nodeVar.valueMax.f))
						{
							node.second.GeneratePreview();
						}
					}
					break;
				case FastNoise::Metadata::MemberVariable::EInt:
					{
						if (ImGui::DragInt(formatName.c_str(), &nodeData->variables[i].i, 0.2f, nodeVar.valueMin.i, nodeVar.valueMax.i))
						{
							node.second.GeneratePreview();
						}
					}
					break;
				case FastNoise::Metadata::MemberVariable::EEnum:
					{
						if (ImGui::Combo(formatName.c_str(), &nodeData->variables[i].i, nodeVar.enumNames.data(), (int)nodeVar.enumNames.size()) ||
							ImGuiExtra::ScrollCombo(&nodeData->variables[i].i, (int)nodeVar.enumNames.size()))
						{
							node.second.GeneratePreview();
						}
					}
					break;
			}

			ImNodes::EndStaticAttribute();
			DoHoverPopup(nodeMetadata->memberVariables[i].description);
		}

		ImGui::PopItemWidth();
		ImNodes::PopAttributeFlag();
		ImNodes::BeginOutputAttribute(node.second.GetOutputAttributeId(), ImNodesPinShape_QuadFilled);

		ImVec2 noiseSize = {(float)Node::NoiseSize, (float)Node::NoiseSize};
		if (mSelectedNode == node.first && !node.second.serialised.empty())
		{
			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImGui::RenderFrame(cursorPos - ImVec2(1, 1), cursorPos + noiseSize + ImVec2(1, 1), IM_COL32(255, 0, 0, 200), false);
		}

		galaxy::ui::imgui_image(node.second.noiseTexture, noiseSize);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			ChangeSelectedNode(node.first);
		}
		ImNodes::EndOutputAttribute();

		ImNodes::EndNode();
	}

	// Do current node links
	for (auto& node : mNodes)
	{
		int attributeId = node.second.GetStartingAttributeId();

		for (FastNoise::NodeData* link : node.second.GetNodeIDLinks())
		{
			if (link)
			{
				ImNodes::Link(attributeId, mNodes.at(link).GetOutputAttributeId(), attributeId);
			}
			attributeId++;
		}
	}
}

FastNoiseNodeEditor::Node& FastNoiseNodeEditor::AddNode(ImVec2 startPos, const FastNoise::Metadata* metadata, bool generatePreview)
{
	FastNoise::NodeData* nodeData = new FastNoise::NodeData(metadata);

	auto newNode = mNodes.try_emplace(nodeData, *this, nodeData, generatePreview);

	ImNodes::SetNodeScreenSpacePos(newNode.first->second.nodeId, startPos);

	if (mNodes.size() == 1)
	{
		ChangeSelectedNode(nodeData);
	}

	return newNode.first->second;
}

bool FastNoiseNodeEditor::AddNodeFromEncodedString(const char* string, ImVec2 nodePos)
{
	std::vector<std::unique_ptr<FastNoise::NodeData>> nodeData;

	if (FastNoise::NodeData* firstNodeData = FastNoise::Metadata::DeserialiseNodeData(string, nodeData))
	{
		for (auto& data : nodeData)
		{
			FastNoise::NodeData* newNodeData = data.get();
			mNodes.emplace(std::piecewise_construct, std::forward_as_tuple(newNodeData), std::forward_as_tuple(*this, std::move(data)));
		}

		if (mNodes.size() == nodeData.size())
		{
			ChangeSelectedNode(firstNodeData);
		}

		Node& firstNode = mNodes.at(firstNodeData);

		ImNodes::SetNodeScreenSpacePos(firstNode.nodeId, nodePos);
		firstNode.AutoPositionChildNodes(nodePos);
		return true;
	}

	return false;
}

void FastNoiseNodeEditor::DoHelp()
{
	ImGui::Text(" Help");
	if (ImGui::IsItemHovered())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.f, 4.f));
		ImGui::BeginTooltip();
		constexpr float alignPx = 110;

		ImGui::TextUnformatted("Add nodes");
		ImGui::SameLine(alignPx);
		ImGui::TextUnformatted("Right mouse click");

		ImGui::TextUnformatted("Pan graph");
		ImGui::SameLine(alignPx);
		ImGui::TextUnformatted("Right mouse drag");

		ImGui::TextUnformatted("Delete node/link");
		ImGui::SameLine(alignPx);
		ImGui::TextUnformatted("Backspace or Delete");

		ImGui::TextUnformatted("Node options");
		ImGui::SameLine(alignPx);
		ImGui::TextUnformatted("Right click node title");

		ImGui::EndTooltip();
		ImGui::PopStyleVar();
	}
}

void FastNoiseNodeEditor::DoContextMenu()
{
	std::string className;
	ImVec2 drag          = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	float distance       = sqrtf(ImDot(drag, drag));
	bool openImportModal = false;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
	if (distance < 5.0f && ImGui::BeginPopupContextWindow("new_node", 1))
	{
		mContextStartPos = ImGui::GetMousePosOnOpeningCurrentPopup();

		if (auto newMetadata = mContextMetadata.front()->DrawUI())
		{
			AddNode(mContextStartPos, newMetadata);
		}

		if (ImGui::BeginMenu("Import"))
		{
			if (ImGui::MenuItem("Encoded Node Tree"))
			{
				openImportModal = true;
			}
			ImGui::Separator();
			for (size_t i = 0; i < sizeof(gDemoNodeTrees) / sizeof(gDemoNodeTrees[0]); i++)
			{
				if (ImGui::MenuItem(gDemoNodeTrees[i][0]))
				{
					AddNodeFromEncodedString(gDemoNodeTrees[i][1], mContextStartPos);
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	if (openImportModal)
	{
		mImportNodeModal = true;
		mImportNodeString.clear();
		ImGui::OpenPopup("New From Encoded Node Tree");
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {5, 5});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {5, 5});
	ImGui::SetNextWindowSize({400, 92}, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_Always, {0.5f, 0.5f});

	if (ImGui::BeginPopupModal("New From Encoded Node Tree", &mImportNodeModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
	{
		if (openImportModal)
		{
			ImGui::SetKeyboardFocusHere();
		}

		bool txtEnter = ImGui::InputText("Base64 String",
			&mImportNodeString,
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank);

		if (txtEnter | ImGui::Button("Create", {100, 30}))
		{
			if (AddNodeFromEncodedString(mImportNodeString.c_str(), mContextStartPos))
			{
				mImportNodeModal = false;
			}
			else
			{
				mImportNodeString = "DESERIALISATION FAILED";
			}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(2);

	if (mDroppedLink)
	{
		ImGui::OpenPopup("new_node_drop");
		mDroppedLink = false;
	}
	if (ImGui::BeginPopup("new_node_drop", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings))
	{
		ImVec2 startPos = ImGui::GetMousePosOnOpeningCurrentPopup();

		auto newMetadata = mContextMetadata.front()->DrawUI([](const FastNoise::Metadata* metadata) {
			return !metadata->memberNodeLookups.empty() || !metadata->memberHybrids.empty();
		});

		if (newMetadata)
		{
			auto& newNode = AddNode(startPos, newMetadata);

			newNode.GetNodeLink(0) = mDroppedLinkNode;
			newNode.GeneratePreview();
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

FastNoise::SmartNode<> FastNoiseNodeEditor::GenerateSelectedPreview()
{
	auto find = mNodes.find(mSelectedNode);

	FastNoise::SmartNode<> generator;

	if (find != mNodes.end())
	{
		generator = FastNoise::NewFromEncodedNodeTree(find->second.serialised.c_str(), mMaxSIMDLevel);

		if (generator)
		{
			mActualSIMDLevel = generator->GetSIMDLevel();
		}
	}

	mNoiseTexture.ReGenerate(generator);

	return generator;
}

FastNoise::OutputMinMax FastNoiseNodeEditor::GenerateNodePreviewNoise(FastNoise::Generator* gen, float* noise)
{
	switch (mNodeGenType)
	{
		case NoiseTexture::GenType_2D:
			return gen->GenUniformGrid2D(noise, Node::NoiseSize / -2, Node::NoiseSize / -2, Node::NoiseSize, Node::NoiseSize, mNodeFrequency, mNodeSeed);

		case NoiseTexture::GenType_2DTiled:
			return gen->GenTileable2D(noise, Node::NoiseSize, Node::NoiseSize, mNodeFrequency, mNodeSeed);

		case NoiseTexture::GenType_3D:
			return gen->GenUniformGrid3D(noise, Node::NoiseSize / -2, Node::NoiseSize / -2, 0, Node::NoiseSize, Node::NoiseSize, 1, mNodeFrequency, mNodeSeed);

		case NoiseTexture::GenType_4D:
			return gen
				->GenUniformGrid4D(noise, Node::NoiseSize / -2, Node::NoiseSize / -2, 0, 0, Node::NoiseSize, Node::NoiseSize, 1, 1, mNodeFrequency, mNodeSeed);
		case NoiseTexture::GenType_Count:
			break;
	}

	return {};
}

FastNoiseNodeEditor::Node* FastNoiseNodeEditor::FindNodeFromId(int id)
{
	auto find = std::find_if(mNodes.begin(), mNodes.end(), [id](const auto& node) {
		return node.second.nodeId == id;
	});

	if (find != mNodes.end())
	{
		return &find->second;
	}

	return nullptr;
}

int FastNoiseNodeEditor::GetFreeNodeId()
{
	static int newNodeId = 0;

	do
	{
		newNodeId = std::max(1, (newNodeId + 1) & (INT_MAX >> Node::AttributeBitCount));

	} while (FindNodeFromId(newNodeId));

	return newNodeId;
}

void FastNoiseNodeEditor::ChangeSelectedNode(FastNoise::NodeData* newId)
{
	mSelectedNode = newId;

	GenerateSelectedPreview();
}

const char* FastNoiseNodeEditor::GetSIMDLevelName(FastSIMD::eLevel lvl)
{
	switch (lvl)
	{
		default:
		case FastSIMD::Level_Null:
			return "NULL";
		case FastSIMD::Level_Scalar:
			return "Scalar";
		case FastSIMD::Level_SSE:
			return "SSE";
		case FastSIMD::Level_SSE2:
			return "SSE2";
		case FastSIMD::Level_SSE3:
			return "SSE3";
		case FastSIMD::Level_SSSE3:
			return "SSSE3";
		case FastSIMD::Level_SSE41:
			return "SSE4.1";
		case FastSIMD::Level_SSE42:
			return "SSE4.2";
		case FastSIMD::Level_AVX:
			return "AVX";
		case FastSIMD::Level_AVX2:
			return "AVX2";
		case FastSIMD::Level_AVX512:
			return "AVX512";
		case FastSIMD::Level_NEON:
			return "NEON";
	}
}