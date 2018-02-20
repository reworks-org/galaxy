//- Common Code For All Addons needed just to ease inclusion as separate files in user code ----------------------
#include "sl/libs/imgui/imgui.h"
#undef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#include "sl/libs/imgui/imgui_internal.h"
//-----------------------------------------------------------------------------------------------------------------


#include "imguidatechooser.h"
#include <time.h>               // very simple and common plain C header file (it's NOT the c++ <sys/time.h>). If not available it's probably better to implement it yourself rather than modifying this file.
#include <ctype.h>  // toupper()

namespace ImGui {


	//struct tm
	//{
	//  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
	//  int tm_min;			/* Minutes.	[0-59] */
	//  int tm_hour;			/* Hours.	[0-23] */
	//  int tm_mday;			/* Day.		[1-31] */
	//  int tm_mon;			/* Month.	[0-11] */
	//  int tm_year;			/* Year	- 1900.  */
	//  int tm_wday;			/* Day of week.	[0-6] */
	//  int tm_yday;			/* Days in year.[0-365]	*/
	//  int tm_isdst;			/* DST.		[-1/0/1]*/

	//# ifdef	__USE_BSD
	//  long int tm_gmtoff;		/* Seconds east of UTC.  */
	//  const char *tm_zone;		/* Timezone abbreviation.  */
	//# else
	//  long int __tm_gmtoff;		/* Seconds east of UTC.  */
	//  const char *__tm_zone;	/* Timezone abbreviation.  */
	//# endif
	//};

	inline static tm GetCurrentDate() {
		time_t now; time(&now);
		return *localtime(&now);
	}
	// Tip: we modify tm (its fields can even be negative!) and then we call this method to retrieve a valid date
	inline static void RecalculateDateDependentFields(tm& date) {
		date.tm_isdst = -1;   // This tries to detect day time savings too
		time_t tmp = mktime(&date);
		date = *localtime(&tmp);
	}
	/*inline static size_t DateToString(tm& d,char* buf,size_t bufferSize)  {
	return strftime(buf,bufferSize,
	//"%a %d %b %Y"
	"%d/%m/%Y"
	,&d);
	}*/


	inline static tm GetDateZero() {
		tm date;
		memset(&date, 0, sizeof(tm));     // Mandatory for emscripten. Please do not remove!
		date.tm_isdst = -1;
		date.tm_sec = 0;		//	 Seconds.	[0-60] (1 leap second)
		date.tm_min = 0;		//	 Minutes.	[0-59]
		date.tm_hour = 0;		//	 Hours.	[0-23]
		date.tm_mday = 0;		//	 Day.		[1-31]
		date.tm_mon = 0;		//	 Month.	[0-11]
		date.tm_year = 0;		//	 Year	- 1900.
		date.tm_wday = 0;     //	 Day of week.	[0-6]
		date.tm_yday = 0;		//	 Days in year.[0-365]
		return date;
	}
	void SetDateZero(tm* date) {
		if (!date) return;
		*date = GetDateZero();
		return;
	}
	void SetDateToday(tm* date) {
		if (!date) return;
		*date = GetCurrentDate();
		return;
	}


	bool DateChooser(const char* label, tm& dateOut, const char* dateFormat, bool closeWhenMouseLeavesIt, bool* pSetStartDateToDateOutThisFrame, const char* leftArrow, const char* rightArrow, const char* upArrowString, const char* downArrowString) {
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();

		if (window->SkipItems)
			return false;

		static tm d = GetDateZero();
		if (pSetStartDateToDateOutThisFrame && *pSetStartDateToDateOutThisFrame) {
			*pSetStartDateToDateOutThisFrame = false;
			if (dateOut.tm_mday == 0) dateOut = GetCurrentDate();
			d = dateOut;
			// move d at the first day of the month: mandatory fo the algo I'll use below
			d.tm_mday = 1;
			RecalculateDateDependentFields(d);  // now d.tm_wday is correct
		}
		else if (dateOut.tm_mday == 0) {
			dateOut = GetCurrentDate();
			d = dateOut;
			// move d at the first day of the month: mandatory fo the algo I'll use below
			d.tm_mday = 1;
			RecalculateDateDependentFields(d);  // now d.tm_wday is correct
		}
		else if (d.tm_mday == 0) {
			d = GetCurrentDate();
			// move d at the first day of the month: mandatory fo the algo I'll use below
			d.tm_mday = 1;
			RecalculateDateDependentFields(d);  // now d.tm_wday is correct
		}
		else if (d.tm_mday != 1) {
			// move d at the first day of the month: mandatory fo the algo I'll use below
			d.tm_mday = 1;
			RecalculateDateDependentFields(d);  // now d.tm_wday is correct
		}

		static const int nameBufferSize = 64;
		static char dayNames[7][nameBufferSize] = { "","","","","","","" };
		static char monthNames[12][nameBufferSize] = { "","","","","","","" };
		static char* pMonthNames[12] = { &monthNames[0][0],&monthNames[1][0],&monthNames[2][0],&monthNames[3][0],&monthNames[4][0],&monthNames[5][0],&monthNames[6][0],&monthNames[7][0],&monthNames[8][0],&monthNames[9][0],&monthNames[10][0],&monthNames[11][0] };
		static int maxMonthWidthIndex = -1;
		if (strlen(dayNames[0]) == 0) {
			tm tmp = GetDateZero();
			for (int i = 0; i<7; i++) {
				tmp.tm_wday = i;
				char* dayName = &dayNames[i][0];
				strftime(dayName, nameBufferSize, "%A", &tmp);
				//fprintf(stderr,"%s\n",dayNames[i]);
				// Warning: This part won't work for multibyte UTF-8 chars:
				// We want to keep the first letter only here, and we want it to be uppercase (because some locales provide it lowercase)
				if (strlen(dayName) == 0) {
					static const char fallbacks[7] = { 'S','M','T','W','T','F','S' };
					dayName[0] = fallbacks[i]; dayName[1] = '\0';
				}
				else {
					dayName[0] = toupper(dayName[0]);
					dayName[1] = '\0';
				}
				//fprintf(stderr,"%s\n",dayNames[i]);

			}
			float maxMonthWidth = 0;
			for (int i = 0; i<12; i++) {
				tmp.tm_mon = i;
				char* monthName = &monthNames[i][0];
				strftime(monthName, nameBufferSize, "%B", &tmp);
				//fprintf(stderr,"%s\n",monthNames[i]);
				// Warning: This part won't work for multibyte UTF-8 chars:
				// We want the first letter to be uppercase (because some locales provide it lowercase)
				if (strlen(monthName) == 0) {
					static const char* fallbacks[12] = { "January","February","March","April","May","June","July","August","September","October","November","December" };
					strcpy(monthName, fallbacks[i]);
				}
				else monthName[0] = toupper(monthName[0]);
				//fprintf(stderr,"%s\n",monthNames[i]);
				float mw = ImGui::CalcTextSize(monthName).x;
				if (maxMonthWidth<mw) {
					maxMonthWidth = mw;
					maxMonthWidthIndex = i;
				}
			}
		}

		ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		const char* arrowLeft = leftArrow ? leftArrow : "<";
		const char* arrowRight = rightArrow ? rightArrow : ">";
		const float arrowLeftWidth = ImGui::CalcTextSize(arrowLeft).x;
		const float arrowRightWidth = ImGui::CalcTextSize(arrowRight).x;

		static char currentText[128]; strftime(currentText, 128, dateFormat, &dateOut);
		const ImVec2 label_size = CalcTextSize(label, NULL, true);
		float widthAdder = 0;
		#   ifdef IMGUIVARIOUSCONTROLS_H_
		widthAdder += 2.f*style.ItemSpacing.x;
		#   endif //IMGUIVARIOUSCONTROLS_H_
		const float widthRequiredByCalendar = 2.f*arrowLeftWidth + 2.f*arrowRightWidth + ImGui::CalcTextSize(monthNames[maxMonthWidthIndex]).x + ImGui::CalcTextSize("9999").x + 50.f + widthAdder;
		const float widthRequiredByTextBox = label_size.x + ImGui::CalcTextSize(currentText).x;// + style.FramePadding.x*2.0f;
		const float w = widthRequiredByCalendar > widthRequiredByTextBox ? widthRequiredByCalendar : widthRequiredByTextBox;//ImGui::CalcItemWidth();

		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y) + style.FramePadding*2.0f);
		const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(style.ItemInnerSpacing.x + label_size.x, 0));
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id)) return false;

		const float arrow_size = (g.FontSize + style.FramePadding.x * 2.0f);
		const bool hovered = ItemHoverable(frame_bb, id);

		bool value_changed = false;
		const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
		RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
		RenderFrame(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y), frame_bb.Max, GetColorU32(hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), true, style.FrameRounding);	// FIXME-ROUNDING
		RenderTriangle(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y) + style.FramePadding, ImGuiDir_Down);

		//RenderTextClipped(frame_bb.Min + style.FramePadding, currentText, NULL, NULL, value_bb.Max);
		RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max, currentText, NULL, NULL);


		if (label_size.x > 0) RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

		ImGuiStorage* storage = ImGui::GetStateStorage();
		const bool isOpenStored = storage->GetBool(id, false);
		bool isOpen = isOpenStored;

		ImGui::PushID((int)id);
		//bool menu_toggled = false;
		if (hovered) {
			//g.HoveredId = id;
			if (g.IO.MouseClicked[0])   isOpen = !isOpen;
			if (isOpen && g.IO.MouseClicked[1]) {
				// reset date when user right clicks the date chooser header whe the dialog is open
				d = GetCurrentDate();
				// move d at the first day of the month: mandatory fo the algo I'll use below
				d.tm_mday = 1;
				RecalculateDateDependentFields(d);  // now d.tm_wday is correct
			}
		}


		if (isOpen) {

			const int height_in_items = 1 + 1 + 1 + 4;
			const int items_count = height_in_items;

			const ImVec2 backup_pos = ImGui::GetCursorPos();
			const float popup_off_x = 0.0f;
			const float popup_height = (label_size.y + 2 * style.ItemSpacing.y) * ImMin(items_count, height_in_items) + (style.FramePadding.y * 3);
			const ImRect popup_rect(ImVec2(frame_bb.Min.x + popup_off_x + ImGui::GetScrollX(), frame_bb.Max.y + ImGui::GetScrollY()), ImVec2(frame_bb.Max.x + popup_off_x + ImGui::GetScrollX(), frame_bb.Max.y + popup_height + ImGui::GetScrollY()));
			ImGui::SetCursorPos(popup_rect.Min - window->Pos);

			// ImGuiWindowFlags_ComboBox is no longer present... we simply remove it and see if it still work...
			const ImGuiWindowFlags flags = /*ImGuiWindowFlags_ComboBox |*/ ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;// | ImGuiWindowFlags_Modal;    // Adding ImGuiWindowFlags_Modal seems to work too...
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_PopupBg]);
			ImGui::BeginChild("#ComboBoxDateChooser", popup_rect.GetSize(), true, flags);
			ImGui::Spacing();

			bool combo_item_active = false;

			static const ImVec4 transparent(1, 1, 1, 0);
			ImGui::PushStyleColor(ImGuiCol_Button, transparent);

			static char yearString[12] = ""; sprintf(yearString, "%d", 1900 + d.tm_year);
			//const float monthPartWidth = arrowLeftWidth + arrowRightWidth + ImGui::CalcTextSize(monthNames[d.tm_mon]).x;
			const float yearPartWidth = arrowLeftWidth + arrowRightWidth + ImGui::CalcTextSize(yearString).x + widthAdder * 0.5f;

			const float oldWindowRounding = style.WindowRounding;
			style.WindowRounding = 0;

			ImGui::PushID(1234);
			if (ImGui::SmallButton(arrowLeft)) { d.tm_mon -= 1; RecalculateDateDependentFields(d); }
			ImGui::SameLine();
			//#       undef  IMGUIVARIOUSCONTROLS_H_
			#       ifndef IMGUIVARIOUSCONTROLS_H_
			ImGui::Text("%s", monthNames[d.tm_mon]);
			#       else //IMGUIVARIOUSCONTROLS_H_
			static ImGui::PopupMenuSimpleParams popupMonthParams;
			ImVec2 menupos(ImGui::GetCursorPos().x + popup_rect.Min.x, ImGui::GetIO().MousePos.y);
			bool previousOpen = popupMonthParams.open;
			popupMonthParams.open |= ImGui::SmallButton(monthNames[d.tm_mon]);
			if (popupMonthParams.open) {
				if (previousOpen != popupMonthParams.open) ImGui::SetNextWindowPos(menupos);
				const int selectedEntry = ImGui::PopupMenuSimple(popupMonthParams, (const char**)pMonthNames, 12, 6, false, NULL, upArrowString, downArrowString);
				if (selectedEntry >= 0) { d.tm_mon = selectedEntry; RecalculateDateDependentFields(d); }
			}
			#       endif //IMGUIVARIOUSCONTROLS_H_
			ImGui::SameLine();
			if (ImGui::SmallButton(arrowRight)) { d.tm_mon += 1; RecalculateDateDependentFields(d); }
			ImGui::PopID();

			ImGui::SameLine(ImGui::GetWindowWidth() - yearPartWidth - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().ItemSpacing.x*4.f);

			ImGui::PushID(1235);
			if (ImGui::SmallButton(arrowLeft)) { d.tm_year -= 1; if (d.tm_year<0) d.tm_year = 0; RecalculateDateDependentFields(d); }
			ImGui::SameLine();

			#       ifndef IMGUIVARIOUSCONTROLS_H_
			ImGui::Text("%d", 1900 + d.tm_year);
			#       else //IMGUIVARIOUSCONTROLS_H_
			static ImGui::PopupMenuSimpleParams popupYearParams;
			menupos = ImVec2(ImGui::GetCursorPos().x + popup_rect.Min.x, ImGui::GetIO().MousePos.y);
			previousOpen = popupYearParams.open;
			popupYearParams.open |= ImGui::SmallButton(yearString);
			if (popupYearParams.open) {
				char years[20][5];
				char* pyears[20];
				for (int i = 0; i<20; i++) { sprintf(&years[i][0], "%d", 1900 + d.tm_year - 10 + i); pyears[i] = &years[i][0]; }
				if (previousOpen != popupYearParams.open) ImGui::SetNextWindowPos(menupos);
				const int selectedEntry = ImGui::PopupMenuSimple(popupYearParams, (const char**)pyears, 20, 6, false, NULL, upArrowString, downArrowString);
				if (selectedEntry >= 0) { if (selectedEntry + d.tm_year - 10 >= 0) { d.tm_year += selectedEntry - 10; RecalculateDateDependentFields(d); } }
			}
			#       endif //IMGUIVARIOUSCONTROLS_H_
			ImGui::SameLine();
			if (ImGui::SmallButton(arrowRight)) { d.tm_year += 1; RecalculateDateDependentFields(d); }
			ImGui::PopID();

			ImGui::Spacing();

			const static int numDaysPerMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
			int maxDayOfCurMonth = numDaysPerMonth[d.tm_mon];   // This could be calculated only when needed (but I guess it's fast in any case...)
			if (maxDayOfCurMonth == 28) {
				const int year = d.tm_year + 1900;
				const bool bis = ((year % 4) == 0) && ((year % 100) != 0 || (year % 400) == 0);
				if (bis) maxDayOfCurMonth = 29;
			}
			static char curDayStr[3] = "";

			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_HeaderHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_HeaderActive]);

			IM_ASSERT(d.tm_mday == 1);    // Otherwise the algo does not work
										  // Display items
			for (int dw = 0; dw<7; dw++) {
				ImGui::BeginGroup();
				if (dw == 0) {
					const ImVec4& tc = style.Colors[ImGuiCol_Text];
					const float l = (tc.x + tc.y + tc.z)*0.33334f;
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(l*2.f>1 ? 1 : l * 2.f, l*.5f, l*.5f, tc.w));
				}
				ImGui::Separator();
				ImGui::Text(" %s", dayNames[dw]);
				ImGui::Separator();
				ImGui::Spacing();
				//-----------------------------------------------------------------------
				int curDay = dw - d.tm_wday;      // tm_wday is in [0,6]. For this to work here d must point to the first day of the month: i.e.: d.tm_mday = 1;
				for (int row = 0; row<7; row++) {
					int cday = curDay + 7 * row;
					if (cday >= 0 && cday<maxDayOfCurMonth) {
						ImGui::PushID(row * 10 + dw);
						if (cday<9) sprintf(curDayStr, " %d", cday + 1);
						else sprintf(curDayStr, "%d", cday + 1);
						if (ImGui::SmallButton(curDayStr)) {
							ClearActiveID();
							//-------------------------
							value_changed = true;
							dateOut = d;
							dateOut.tm_mday = cday + 1;
							RecalculateDateDependentFields(dateOut);
							d = dateOut;
							//fprintf(stderr,"Chosen date: %d-%d-%d/n",dateOut.tm_mday,dateOut.tm_mon+1,dateOut.tm_year+1900);
							combo_item_active = true;
							isOpen = false;
						}
						combo_item_active |= ImGui::IsItemActive();
						ImGui::PopID();
					}
					else ImGui::Text(" ");
				}
				//-----------------------------------------------------------------------
				ImGui::Separator();
				if (dw == 0) ImGui::PopStyleColor();
				ImGui::EndGroup();
				if (dw != 6) ImGui::SameLine(ImGui::GetWindowWidth() - (6 - dw)*(ImGui::GetWindowWidth() / 7.f));
			}

			style.WindowRounding = oldWindowRounding;
			ImGui::PopStyleColor(2);

			if (closeWhenMouseLeavesIt || (!combo_item_active && g.ActiveId != 0))
			{
				const float distance = g.FontSize*1.75f;//1.3334f;//24;
														//fprintf(stderr,"%1.f",distance);
				ImVec2 pos = ImGui::GetWindowPos(); pos.x -= distance; pos.y -= distance;
				ImVec2 size = ImGui::GetWindowSize(); size.x += 2.f*distance; size.y += 2.f*distance;
				const ImVec2& mousePos = ImGui::GetIO().MousePos;
				if (mousePos.x<pos.x || mousePos.y<pos.y || mousePos.x>pos.x + size.x || mousePos.y>pos.y + size.y) {
					isOpen = false;
					d = dateOut;
					//fprintf(stderr,"Leaving DateChooser\n");
				}
			}

			ImGui::PopStyleColor();

			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::SetCursorPos(backup_pos);

		}

		ImGui::PopID();

		if (isOpen != isOpenStored) storage->SetBool(id, isOpen);

		//if (value_changed) d.tm_mday=0;    //reset d
		return value_changed;

	}


	void TestDateChooser(const char* dateFormat, bool closeWhenMouseLeavesIt, const char* leftArrow, const char* rightArrow, const char* upArrowString, const char* downArrowString) {
		static tm date = GetDateZero();
		if (DateChooser("DateChooser##id", date, dateFormat, closeWhenMouseLeavesIt, NULL, leftArrow, rightArrow, upArrowString, downArrowString)) {
			// Do spmething with date
		}
	}



} // namespace ImGui
