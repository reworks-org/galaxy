// dear imgui: wrappers for C++ standard library (STL) types (std::string, etc.)
// This is also an example of how you may wrap your own similar types.

// Compatibility:
// - std::string support is only guaranteed to work from C++11.
//   If you try to use it pre-C++11, please share your findings (w/ info about compiler/architecture)

// Changelog:
// - v0.10: Initial version. Added InputText() / InputTextMultiline() calls with std::string

#include "imgui.h"
#include "imgui_stdlib.h"

struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool ImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool ImGui::InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}

bool ImGui::InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

inline bool ItemsGetterVectorOfStrings(void* userData, int index, const char** items)
{
	const auto& itemNames = *static_cast<std::vector<std::string>*>(userData);
	*items = itemNames[index].data();
	return true;
};

bool ImGui::ListBox(const char* label, int* current_item, const std::vector<std::string>& itemNames, int height_in_items)
{
	return ImGui::ListBox(label, current_item, ItemsGetterVectorOfStrings, const_cast<void*>(static_cast<const void*>(&itemNames)), itemNames.size(), height_in_items);
}

bool ImGui::Combo(const char* label, int* current_item, const std::vector<std::string>& itemNames, int height_in_items)
{
	return ImGui::Combo(label, current_item, ItemsGetterVectorOfStrings, const_cast<void*>(static_cast<const void*>(&itemNames)), itemNames.size(), height_in_items);
}

// Template function for text buffer contorl for imgui text input fields
template<typename Container,				// Buffer's container type, std::string or std::vector<char> in this case.
	int sub_buffer_size = 256,			// Buffer resize is triggerd after this many char inputs or removals. Also, system supports this many character inputs or removals at once.
	int forward_sub_buffer_cnt = 2>	// Buffer is extended forward by this many sub-buffers. Since ImGui input fields locks buffer while editing, this determines how many characters can be inserted in one session, that is, the text field is active.
	inline void AutosizeTextBuffer(Container& buf)
{
	static_assert(sub_buffer_size > 0 && forward_sub_buffer_cnt > 1, "Invalid buffering parameters.");
	constexpr int offset3 = sub_buffer_size * (forward_sub_buffer_cnt + 1);	// above offset3, we expand
	constexpr int offset2 = sub_buffer_size * (forward_sub_buffer_cnt + 0);	// between offset2 and offset3, nothing
	constexpr int offset1 = sub_buffer_size * (forward_sub_buffer_cnt - 1);	// below offset1, shring buffer
	if (buf.size() < offset2) buf.resize(offset2, '\0');	//minimum size
	else
	{
		int shift = max(buf.size(), offset3) - offset3;
		int slen = std::char_traits<char>::length(buf.data() + shift) + shift;
		if (slen > buf.size() - offset1)
			buf.resize(buf.size() + sub_buffer_size, '\0'); //expand
		else if (slen < buf.capacity())
			buf.resize(buf.size() - sub_buffer_size, '\0'); //shrink
	}
	if (buf.size() * 2 <= buf.capacity()) buf.shrink_to_fit();	//keep memory in check
}

struct AutoBufferTextInputCallbackData
{
	ImGuiTextEditCallback callback;
	void* user_data;
	ImGuiInputTextFlags flags;
	int text_length = -1;
};

int autoBufferTextInputCallbackFunction(ImGuiTextEditCallbackData* data)
{
	auto* mydata = static_cast<AutoBufferTextInputCallbackData*>(data->UserData);
	mydata->text_length = data->BufTextLen;		//steal text length
	if (mydata->callback != nullptr && (data->EventFlag & mydata->flags))
	{
		data->UserData = mydata->user_data;		//previous callback needs to use this
		return mydata->callback(data);			//return what callback does
	}
	else return 0;
}

template<std::size_t padding = 512, typename Container> //can be used for std::vector<char> as well, but it is probably not needed
void autoBufferResize1(Container& buf)
{
	if (buf.size() < padding) //fix initial state
	{
		buf.resize(buf.size() + padding);
		buf.resize(buf.capacity());
	}
}

template<std::size_t padding = 512, typename Container> //can be used for std::vector<char> as well, but it is probably not needed
void autoBufferResize2(Container& buf, int textLength)
{
	if (textLength != -1) // if textLength == 0, then either there was no update, or the there is no text
	{
		std::size_t padded_size = textLength * 3 / 2 + padding;
		if (buf.size() != padded_size)
		{
			buf.resize(padded_size);
			if (2 * buf.size() < buf.capacity())
				buf.shrink_to_fit();
			else if (buf.size() != buf.capacity())
				buf.resize(buf.capacity());
		}
	}
}