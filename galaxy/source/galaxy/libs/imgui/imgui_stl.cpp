// imgui_stl.cpp
// Wrappers for C++ standard library (STL) types (std::string, etc.)
// This is also an example of how you may wrap your own similar types.

// Compatibility:
// - std::string support is only guaranteed to work from C++11. 
//   If you try to use it pre-C++11, please share your findings (w/ info about compiler/architecture)

#include "imgui.h"
#include "imgui_stl.h"

inline bool ItemsGetterVectorOfStrings(void *userData, int index, const char **items)
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
template<typename Container,                // Buffer's container type, std::string or std::vector<char> in this case.
    int sub_buffer_size = 256,          // Buffer resize is triggerd after this many char inputs or removals. Also, system supports this many character inputs or removals at once.
    int forward_sub_buffer_cnt = 2> // Buffer is extended forward by this many sub-buffers. Since ImGui input fields locks buffer while editing, this determines how many characters can be inserted in one session, that is, the text field is active.
    inline void AutosizeTextBuffer(Container &buf)
{
    static_assert(sub_buffer_size > 0 && forward_sub_buffer_cnt > 1, "Invalid buffering parameters.");
    constexpr int offset3 = sub_buffer_size*(forward_sub_buffer_cnt + 1);   // above offset3, we expand
    constexpr int offset2 = sub_buffer_size*(forward_sub_buffer_cnt + 0);   // between offset2 and offset3, nothing
    constexpr int offset1 = sub_buffer_size*(forward_sub_buffer_cnt - 1);   // below offset1, shring buffer
    if (buf.size() < offset2) buf.resize(offset2, '\0');    //minimum size
    else
    {
        int shift = max(buf.size(), offset3) - offset3;
        int slen = std::char_traits<char>::length(buf.data() + shift) + shift;
        if (slen > buf.size() - offset1)
            buf.resize(buf.size() + sub_buffer_size, '\0'); //expand
        else if (slen < buf.capacity())
            buf.resize(buf.size() - sub_buffer_size, '\0'); //shrink
    }
    if (buf.size() * 2 <= buf.capacity()) buf.shrink_to_fit();  //keep memory in check
}

struct AutoBufferTextInputCallbackData
{
    ImGuiTextEditCallback callback;
    void* user_data;
    ImGuiInputTextFlags flags;
    int text_length = -1;
};

int autoBufferTextInputCallbackFunction(ImGuiTextEditCallbackData *data)
{
    auto *mydata = static_cast<AutoBufferTextInputCallbackData*>(data->UserData);
    mydata->text_length = data->BufTextLen;     //steal text length
    if (mydata->callback != nullptr && (data->EventFlag & mydata->flags))
    {
        data->UserData = mydata->user_data;     //previous callback needs to use this
        return mydata->callback(data);          //return what callback does
    }
    else return 0;
}

template<std::size_t padding = 512, typename Container> //can be used for std::vector<char> as well, but it is probably not needed
void autoBufferResize1(Container &buf)
{
    if (buf.size() < padding) //fix initial state
    {
        buf.resize(buf.size() + padding);
        buf.resize(buf.capacity());
    }
}

template<std::size_t padding = 512, typename Container> //can be used for std::vector<char> as well, but it is probably not needed
void autoBufferResize2(Container &buf, int textLength)
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

bool ImGui::InputText(const char* label, std::string &buf, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    autoBufferResize1<256>(buf);
    AutoBufferTextInputCallbackData mydata = { callback , user_data, flags, -1 };
    bool result = ImGui::InputText(label, &buf[0], buf.size(), flags | ImGuiInputTextFlags_CallbackAlways,
                                   autoBufferTextInputCallbackFunction, static_cast<void*>(&mydata));
    autoBufferResize2<256>(buf, mydata.text_length);
    return result;
}

bool ImGui::InputTextMultiline(const char* label, std::string& buf, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    autoBufferResize1<1024>(buf);
    AutoBufferTextInputCallbackData mydata = { callback , user_data, flags, -1};
    bool result = ImGui::InputTextMultiline(label, &buf[0], buf.size(), size, flags | ImGuiInputTextFlags_CallbackAlways,
                                            autoBufferTextInputCallbackFunction, static_cast<void*>(&mydata));
    autoBufferResize2<1024>(buf, mydata.text_length);
    return result;
}
/*
bool ImGui::InputText(const char* label, std::vector<char> &buf, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
autoBufferResize1<256>(buf);
AutoBufferTextInputCallbackData mydata = { callback , user_data, flags, -1 };
bool result = ImGui::InputText(label, &buf[0], buf.size(), flags | ImGuiInputTextFlags_CallbackAlways,
autoBufferTextInputCallbackFunction, static_cast<void*>(&mydata));
autoBufferResize2<256>(buf, mydata.text_length);
return result;
}
bool ImGui::InputTextMultiline(const char* label, std::vector<char>& buf, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
autoBufferResize1<1024>(buf);
AutoBufferTextInputCallbackData mydata = { callback , user_data, flags, -1 };
bool result = ImGui::InputTextMultiline(label, &buf[0], buf.size(), size, flags | ImGuiInputTextFlags_CallbackAlways,
autoBufferTextInputCallbackFunction, static_cast<void*>(&mydata));
autoBufferResize2<1024>(buf, mydata.text_length);
return result;
}*/

IMGUI_API void ImGui::PlotLines(const char* label, const std::vector<float> &values, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride_in_floats)
{
    ImGui::PlotLines(label, values.data(), values.size() / stride_in_floats, 0, overlay_text, scale_min, scale_max, graph_size, stride_in_floats * sizeof(float));
}
IMGUI_API void ImGui::PlotHistogram(const char* label, const std::vector<float> &values, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride_in_floats)
{
    ImGui::PlotHistogram(label, values.data(), values.size() / stride_in_floats, 0, overlay_text, scale_min, scale_max, graph_size, stride_in_floats*sizeof(float));
}