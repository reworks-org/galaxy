#define FTS_FUZZY_MATCH_IMPLEMENTATION

#include "fts_fuzzy_match.h"
#include "imgui_combo_autoselect.h"

/*
 // Demo:
	static ImGui::ComboAutoSelectData data = {{
					"",
					"AnimGraphNode_CopyBone",
					"ce skipaa",
					"ce skipscreen",
					"ce skipsplash",
					"ce skipsplashscreen",
					"client_unit.cpp",
					"letrograd",
					"level",
					"leveler",
					"MacroCallback.cpp",
					"Miskatonic university",
					"MockAI.h",
					"MockGameplayTasks.h",
					"MovieSceneColorTrack.cpp",
					"r.maxfps",
					"r.maxsteadyfps",
					"reboot",
					"rescale",
					"reset",
					"resource",
					"restart",
					"retrocomputer",
					"retrograd",
					"return",
					"slomo 10",
					"SVisualLoggerLogsList.h",
					"The Black Knight",
	}};
	if (ImGui::ComboAutoSelect("my combofilter", data)) {
		// selection occurred
	}
	ImGui::Text("Selection: %s, index = %d", data.input, data.index);
*/

namespace ImGui
{
	bool ComboAutoSelectComplex(const char* label,
		char*                               buffer,
		int                                 bufferlen,
		int*                                current_item,
		bool                                (*items_getter)(void*, int, const char**),
		void*                               data,
		int                                 items_count,
		ImGuiComboFlags                     flags);
}

static int fuzzy_search(const char* needle, void* data)
{
	auto& items = *(std::vector<std::string>*)data;
	for (int i = 0; i < (int)items.size(); i++)
	{
		auto haystack = items[i].c_str();
		// empty
		if (!needle[0])
		{
			if (!haystack[0])
				return i;
			continue;
		}
		// exact match
		if (strstr(haystack, needle))
			return i;
		// fuzzy match
		if (fts::fuzzy_match_simple(haystack, needle))
			return i;
	}
	return -1;
}

bool ImGui::ComboAutoSelectComplex(const char* label,
	char*                                      input,
	int                                        inputlen,
	int*                                       current_item,
	bool                                       (*items_getter)(void*, int, const char**),
	void*                                      data,
	int                                        items_count,
	ImGuiComboFlags                            flags)
{
	// Always consume the SetNextWindowSizeConstraint() call in our early return paths
	ImGuiContext& g = *GImGui;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	// Call the getter to obtain the preview string which is a parameter to BeginCombo()

	const ImGuiID popupId              = window->GetID(label);
	bool          popupIsAlreadyOpened = IsPopupOpen(popupId, 0); // ImGuiPopupFlags_AnyPopupLevel);
	const char*   sActiveidxValue1     = NULL;
	items_getter(data, *current_item, &sActiveidxValue1);
	bool popupNeedsToBeOpened = (input[0] != 0) && (sActiveidxValue1 && strcmp(input, sActiveidxValue1));
	bool popupJustOpened      = false;

	IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) !=
			  (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

	const ImGuiStyle& style = g.Style;

	const float  arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const float  expected_w = CalcItemWidth();
	const float  w          = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : expected_w;
	const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(frame_bb.Min, ImVec2((label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f) + frame_bb.Max.x, frame_bb.Max.y));
	const float  value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, popupId, &frame_bb))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(frame_bb, popupId, &hovered, &held);

	if (!popupIsAlreadyOpened)
	{
		const ImU32 frame_col = GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		RenderNavHighlight(frame_bb, popupId);
		if (!(flags & ImGuiComboFlags_NoPreview))
			window->DrawList->AddRectFilled(frame_bb.Min,
				ImVec2(value_x2, frame_bb.Max.y),
				frame_col,
				style.FrameRounding,
				(flags & ImGuiComboFlags_NoArrowButton) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft);
	}
	if (!(flags & ImGuiComboFlags_NoArrowButton))
	{
		ImU32 bg_col   = GetColorU32((popupIsAlreadyOpened || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImU32 text_col = GetColorU32(ImGuiCol_Text);
		window->DrawList->AddRectFilled(ImVec2(value_x2, frame_bb.Min.y),
			frame_bb.Max,
			bg_col,
			style.FrameRounding,
			(w <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);
		if (value_x2 + arrow_size - style.FramePadding.x <= frame_bb.Max.x)
			RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), text_col, ImGuiDir_Down, 1.0f);
	}

	if (!popupIsAlreadyOpened)
	{
		RenderFrameBorder(frame_bb.Min, frame_bb.Max, style.FrameRounding);
		if (input != NULL && !(flags & ImGuiComboFlags_NoPreview))
		{
			RenderTextClipped(ImVec2(frame_bb.Min.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y),
				ImVec2(value_x2, frame_bb.Max.y),
				input,
				NULL,
				NULL,
				ImVec2(0.0f, 0.0f));
		}

		if ((pressed || g.NavActivateId == popupId || popupNeedsToBeOpened) && !popupIsAlreadyOpened)
		{
			if (window->DC.NavLayerCurrent == 0)
				window->NavLastIds[0] = popupId;
			OpenPopupEx(popupId);
			popupIsAlreadyOpened = true;
			popupJustOpened      = true;
		}
	}

	if (label_size.x > 0)
	{
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
	}

	if (!popupIsAlreadyOpened)
	{
		return false;
	}

	const float totalWMinusArrow = w - arrow_size;

	struct ImGuiSizeCallbackWrapper
	{
		static void sizeCallback(ImGuiSizeCallbackData* data)
		{
			float* totalWMinusArrow = (float*)(data->UserData);
			data->DesiredSize       = ImVec2(*totalWMinusArrow, 200.f);
		}
	};

	SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(totalWMinusArrow, 150.f), ImGuiSizeCallbackWrapper::sizeCallback, (void*)&totalWMinusArrow);

	char name[16];
	ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

	// Peek into expected window size so we can position it
	if (ImGuiWindow* popup_window = FindWindowByName(name))
	{
		if (popup_window->WasActive)
		{
			ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
			if (flags & ImGuiComboFlags_PopupAlignLeft)
				popup_window->AutoPosLastDirection = ImGuiDir_Left;
			ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
			ImVec2 pos     = FindBestWindowPosForPopupEx(frame_bb.GetBL(),
                size_expected,
                &popup_window->AutoPosLastDirection,
                r_outer,
                frame_bb,
                ImGuiPopupPositionPolicy_ComboBox);

			pos.y -= label_size.y + style.FramePadding.y * 2.0f;

			SetNextWindowPos(pos);
		}
	}

	// Horizontally align ourselves with the framed text
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	// PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
	bool ret = Begin(name, NULL, window_flags);

	PushItemWidth(GetWindowWidth());
	SetCursorPos(ImVec2(0.f, window->DC.CurrLineTextBaseOffset));
	if (popupJustOpened)
	{
		SetKeyboardFocusHere(0);
	}

	bool done =
		InputTextEx("##inputText", NULL, input, inputlen, ImVec2(0, 0), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL);
	PopItemWidth();

	if (!ret)
	{
		EndChild();
		PopItemWidth();
		EndPopup();
		IM_ASSERT(0); // This should never happen as we tested for IsPopupOpen() above
		return false;
	}

	ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus; // 0; //ImGuiWindowFlags_HorizontalScrollbar
	BeginChild("ChildL", ImVec2(GetContentRegionAvail().x, GetContentRegionAvail().y), false, window_flags2);

	bool selectionChanged = false;
	if (input[0] != '\0')
	{
		int new_idx      = fuzzy_search(input, data);
		int idx          = new_idx >= 0 ? new_idx : *current_item;
		selectionChanged = *current_item != idx;
		*current_item    = idx;
	}

	bool arrowScroll = false;
	// int arrowScrollIdx = *current_item;

	if (IsKeyPressed(ImGuiKey_UpArrow))
	{
		if (*current_item > 0)
		{
			*current_item -= 1;
			arrowScroll    = true;
			SetWindowFocus();
		}
	}
	if (IsKeyPressed(ImGuiKey_DownArrow))
	{
		if (*current_item >= -1 && *current_item < items_count - 1)
		{
			*current_item += 1;
			arrowScroll    = true;
			SetWindowFocus();
		}
	}

	// select the first match
	if (IsKeyPressed(ImGuiKey_Enter))
	{
		arrowScroll   = true;
		*current_item = fuzzy_search(input, data);
		if (*current_item < 0)
			*input = 0;
		CloseCurrentPopup();
	}

	if (IsKeyPressed(ImGuiKey_Backspace))
	{
		*current_item    = fuzzy_search(input, data);
		selectionChanged = true;
	}

	if (done && !arrowScroll)
	{
		CloseCurrentPopup();
	}

	bool done2 = false;

	for (int n = 0; n < items_count; n++)
	{
		bool is_selected = n == *current_item;
		if (is_selected && (IsWindowAppearing() || selectionChanged))
		{
			SetScrollHereY();
		}

		if (is_selected && arrowScroll)
		{
			SetScrollHereY();
		}

		const char* select_value = NULL;
		items_getter(data, n, &select_value);

		// allow empty item
		char item_id[128];
		ImFormatString(item_id, sizeof(item_id), "%s##item_%02d", select_value, n);
		if (Selectable(item_id, is_selected))
		{
			selectionChanged = *current_item != n;
			*current_item    = n;
			strncpy(input, select_value, inputlen);
			CloseCurrentPopup();
			done2 = true;
		}
	}

	if (arrowScroll && *current_item > -1)
	{
		const char* sActiveidxValue2 = NULL;
		items_getter(data, *current_item, &sActiveidxValue2);
		strncpy(input, sActiveidxValue2, inputlen);
		ImGuiWindow*         wnd   = FindWindowByName(name);
		const ImGuiID        id    = wnd->GetID("##inputText");
		ImGuiInputTextState* state = GetInputTextState(id);

		const char* buf_end = NULL;
		state->CurLenW      = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, input, NULL, &buf_end);
		state->CurLenA      = (int)(buf_end - input);
		state->CursorClamp();
	}

	EndChild();
	EndPopup();

	const char* sActiveidxValue3 = NULL;
	items_getter(data, *current_item, &sActiveidxValue3);
	bool ret1 = (selectionChanged && (sActiveidxValue3 && !strcmp(sActiveidxValue3, input)));

	bool widgetRet = done || done2 || ret1;

	return widgetRet;
}

static bool vector_item_getter(void* data, int n, const char** out_str)
{
	auto& items = *(std::vector<std::string>*)data;
	if (n >= 0 && n < (int)items.size())
	{
		*out_str = items[n].c_str();
		return true;
	}
	return false;
}

bool ImGui::ComboAutoSelect(const char* label, ImGui::ComboAutoSelectData& data, ImGuiComboFlags flags)
{
	return ComboAutoSelectComplex(label,
		data.input,
		sizeof(data.input) - 1,
		&data.index,
		vector_item_getter,
		(void*)&data.items,
		(int)data.items.size(),
		flags);
}
