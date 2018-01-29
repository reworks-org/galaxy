#pragma once
#include <imgui/imgui.h>
#include <type_traits>
#include <utility>
#include <string>
#include "core.h"

#ifndef IMGUI_AUTO_INPUT_FLOAT1
#define IMGUI_AUTO_INPUT_FLOAT1 ImGui::DragFloat
#endif
#ifndef IMGUI_AUTO_INPUT_FLOAT2
#define IMGUI_AUTO_INPUT_FLOAT2 ImGui::DragFloat2
#endif
#ifndef IMGUI_AUTO_INPUT_FLOAT3
#define IMGUI_AUTO_INPUT_FLOAT3 ImGui::DragFloat3
#endif
#ifndef IMGUI_AUTO_INPUT_FLOAT4
#define IMGUI_AUTO_INPUT_FLOAT4 ImGui::DragFloat4
#endif
#ifndef IMGUI_AUTO_INPUT_INT1
#define IMGUI_AUTO_INPUT_INT1 ImGui::InputInt
#endif
#ifndef IMGUI_AUTO_INPUT_INT2
#define IMGUI_AUTO_INPUT_INT2 ImGui::InputInt2
#endif
#ifndef IMGUI_AUTO_INPUT_INT3
#define IMGUI_AUTO_INPUT_INT3 ImGui::InputInt3
#endif
#ifndef IMGUI_AUTO_INPUT_INT4
#define IMGUI_AUTO_INPUT_INT4 ImGui::InputInt4
#endif
#ifndef IMGUI_AUTO_NULLPTR_COLOR
#define IMGUI_AUTO_NULLPTR_COLOR ImVec4(1.0,0.5,0.5,1.0)
#endif

//		SPECIALIZATIONS
//		===============

#pragma region STRINGS

IMGUI_AUTO_DEFINE_BEGIN(template<>, const char*)
	if (name.empty())	ImGui::TextUnformatted(var);
	else			ImGui::Text("%s=%s", name.c_str(), var);
IMGUI_AUTO_DEFINE_END

IMGUI_AUTO_DEFINE_BEGIN_P((template<std::size_t N>), (const detail::c_array_t<char, N>))
	if (name.empty())	ImGui::TextUnformatted(var, var+N-1);
	else			ImGui::Text("%s=%s", name.c_str(), var);
IMGUI_AUTO_DEFINE_END

IMGUI_AUTO_DEFINE_INLINE(template<>, char*,				const char* tmp = var; ImGui::Auto_t<const char*>::Auto(tmp, name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, char* const,		const char* tmp = var; ImGui::Auto_t<const char*>::Auto(tmp, name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const char* const,	const char* tmp = var; ImGui::Auto_t<const char*>::Auto(tmp, name);)
IMGUI_AUTO_DEFINE_BEGIN(template<>, std::string)
	const std::size_t lines = var.find('\n');
	if (var.find('\n') != std::string::npos)	ImGui::InputTextMultiline(name.c_str(), var);
	else									    ImGui::InputText(name.c_str(), var);
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<>, const std::string)
	if(name.empty())	ImGui::TextUnformatted(var.c_str(), var.c_str()+var.length());
	else				ImGui::Text("%s=%s", name.c_str(), var.c_str());
IMGUI_AUTO_DEFINE_END

#pragma endregion

#pragma region NUMBERS

IMGUI_AUTO_DEFINE_INLINE(template<>, float,				IMGUI_AUTO_INPUT_FLOAT1(name.c_str(), &var);)
IMGUI_AUTO_DEFINE_INLINE(template<>, int,				IMGUI_AUTO_INPUT_INT1(name.c_str(), &var);)
IMGUI_AUTO_DEFINE_INLINE(template<>, unsigned int,		IMGUI_AUTO_INPUT_INT1(name.c_str(), (int*)&var);)
IMGUI_AUTO_DEFINE_INLINE(template<>, bool,				ImGui::Checkbox(name.c_str(), &var);)
IMGUI_AUTO_DEFINE_INLINE(template<>, ImVec2,			IMGUI_AUTO_INPUT_FLOAT2(name.c_str(), &var.x);)
IMGUI_AUTO_DEFINE_INLINE(template<>, ImVec4,			IMGUI_AUTO_INPUT_FLOAT4(name.c_str(), &var.x);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const float,		ImGui::Auto_t<const std::string>::Auto(std::to_string(var), name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const int,			ImGui::Auto_t<const std::string>::Auto(std::to_string(var), name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const unsigned,	ImGui::Auto_t<const std::string>::Auto(std::to_string(var), name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const bool,		ImGui::Auto_t<const std::string>::Auto(std::to_string(var), name);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const ImVec2,		ImGui::Text("%s(%f,%f)",		(name.empty() ? "": name+"=").c_str(), var.x, var.y);)
IMGUI_AUTO_DEFINE_INLINE(template<>, const ImVec4,		ImGui::Text("%s(%f,%f,%f,%f)",	(name.empty() ? "": name+"=").c_str(), var.x, var.y, var.z, var.w);)

IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<float,1>),IMGUI_AUTO_INPUT_FLOAT1(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<float,1>),ImGui::Text("%s%f", (name.empty() ? "": name+"=").c_str(), var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<float,2>),IMGUI_AUTO_INPUT_FLOAT2(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<float,2>),ImGui::Text("%s(%f,%f)", (name.empty() ? "": name+"=").c_str(), var[0],var[1]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<float,3>),IMGUI_AUTO_INPUT_FLOAT3(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<float,3>),ImGui::Text("%s(%f,%f,%f)", (name.empty() ? "": name+"=").c_str(), var[0], var[1], var[2]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<float,4>),IMGUI_AUTO_INPUT_FLOAT4(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<float,4>),ImGui::Text("%s(%f,%f,%f,%f)", (name.empty() ? "": name+"=").c_str(), var[0], var[1], var[2], var[3]);)

IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<int,1>),IMGUI_AUTO_INPUT_INT1(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<int,1>),ImGui::Text("%s%d", (name.empty() ? "": name+"=").c_str(), var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<int,2>),IMGUI_AUTO_INPUT_INT2(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<int,2>),ImGui::Text("%s(%d,%d)", (name.empty() ? "": name+"=").c_str(), var[0],var[1]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<int,3>),IMGUI_AUTO_INPUT_INT3(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<int,3>),ImGui::Text("%s(%d,%d,%d)", (name.empty() ? "": name+"=").c_str(), var[0], var[1], var[2]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(      detail::c_array_t<int,4>),IMGUI_AUTO_INPUT_INT4(name.c_str(), &var[0]);)
IMGUI_AUTO_DEFINE_INLINE_P((template<>),(const detail::c_array_t<int,4>),ImGui::Text("%s(%d,%d,%d,%d)", (name.empty() ? "": name+"=").c_str(), var[0], var[1], var[2], var[3]);)

#pragma endregion

#pragma region POINTERS and ARRAYS
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, T*)
if (var != nullptr) ImGui::detail::AutoExpand<T>("Pointer " + name, *var);
else	ImGui::TextColored(IMGUI_AUTO_NULLPTR_COLOR, "%s=NULL", name.c_str());
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, T* const)
if (var != nullptr) ImGui::detail::AutoExpand<T>("Pointer " + name, *var);
else	ImGui::TextColored(IMGUI_AUTO_NULLPTR_COLOR, "%s=NULL", name.c_str());
IMGUI_AUTO_DEFINE_END
#ifdef _ARRAY_
IMGUI_AUTO_DEFINE_INLINE_P((template<typename T, std::size_t N>), (		 std::array<T, N>),ImGui::detail::AutoContainerValues("array " + name, var);)
IMGUI_AUTO_DEFINE_INLINE_P((template<typename T, std::size_t N>), (const std::array<T, N>),ImGui::detail::AutoContainerValues("array " + name, var);)
IMGUI_AUTO_DEFINE_INLINE_P((template<typename T, std::size_t N>), (detail::c_array_t<T, N>), ImGui::detail::AutoContainerValues("Array " + name, *(std::array<T, N>*)(&var));)
IMGUI_AUTO_DEFINE_INLINE_P((template<typename T, std::size_t N>), (const detail::c_array_t<T, N>), ImGui::detail::AutoContainerValues("Array " + name, *(const std::array<T, N>*)(&var));)
#endif

#pragma endregion

#pragma region PAIRS and TUPLES

IMGUI_AUTO_DEFINE_BEGIN_P((template<typename T1, typename T2>), (std::pair<T1,T2>))
if ((std::is_fundamental_v<T1> || std::is_same_v<std::string, T1>) && (std::is_fundamental_v<T2> || std::is_same_v<std::string, T2>))
{
	float width = ImGui::CalcItemWidth();
	ImGui::PushItemWidth(width*0.4-10); //a bit less than half
	ImGui::detail::AutoExpand<T1>(name + ".first", var.first);
	ImGui::SameLine();
	ImGui::detail::AutoExpand<T2>(name + ".second", var.second);
	ImGui::PopItemWidth();
}
else
{
	ImGui::detail::AutoExpand<T1>(name + ".first", var.first);
	ImGui::detail::AutoExpand<T2>(name + ".second", var.second);
}

IMGUI_AUTO_DEFINE_END

IMGUI_AUTO_DEFINE_BEGIN_P((template<typename T1, typename T2>), (const std::pair<T1,T2>))
	ImGui::detail::AutoExpand<const T1>(name + ".first", var.first);
	if (std::is_fundamental_v<T1> && std::is_fundamental_v<T2>) ImGui::SameLine();
	ImGui::detail::AutoExpand<const T2>(name + ".second", var.second);
IMGUI_AUTO_DEFINE_END

#ifdef _TUPLE_
IMGUI_AUTO_DEFINE_INLINE(template<typename ...Args>,		std::tuple<Args...>, ImGui::detail::AutoTuple("Tuple " + name, var);)
IMGUI_AUTO_DEFINE_INLINE(template<typename ...Args>, const	std::tuple<Args...>, ImGui::detail::AutoTuple("Tuple " + name, var);)
#endif //_TUPLE_

#pragma endregion

#pragma region CONTAINERS
#ifdef _VECTOR_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::vector<T>)
	if(ImGui::detail::AutoContainerValues< std::vector<T> >("Vector " + name, var))
	{
		ImGui::PushID(name.c_str());	ImGui::Indent();
		ImGui::detail::AutoContainerPushBackButton(var); if (!var.empty()) ImGui::SameLine();
		ImGui::detail::AutoContainerPopBackButton(var);
		ImGui::PopID();					ImGui::Unindent();
	}
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<>, std::vector<bool>)
	if(ImGui::detail::AutoContainerTreeNode< std::vector<bool> >("Vector " + name, var))
	{
		ImGui::Indent();
		for (int i = 0; i < var.size(); ++i)
		{
			bool b = var[i];
			ImGui::Bullet();
			ImGui::Auto_t<bool>::Auto(b, '['+std::to_string(i)+']');
			var[i] = b;
		}
		ImGui::PushID(name.c_str());	ImGui::Indent();
		ImGui::detail::AutoContainerPushBackButton(var); if (!var.empty()) ImGui::SameLine();
		ImGui::detail::AutoContainerPopBackButton(var);
		ImGui::PopID();					ImGui::Unindent();
		ImGui::Unindent();
	}
IMGUI_AUTO_DEFINE_END

IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::vector<T>)
	ImGui::detail::AutoContainerValues< const std::vector<T> >("Vector " + name, var);
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<>, const std::vector<bool>)
	if (ImGui::detail::AutoContainerTreeNode< const std::vector<bool> >("Vector " + name, var))
	{
		ImGui::Indent();
		for (int i = 0; i < var.size(); ++i)
		{
			ImGui::Bullet();
			ImGui::Auto_t<const bool>::Auto(var[i], '[' + std::to_string(i) + ']');
		}
		ImGui::Unindent();
	}
IMGUI_AUTO_DEFINE_END
#endif

#ifdef _LIST_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::list<T>)
if(ImGui::detail::AutoContainerValues< std::list<T> >("List " + name, var))
{
	ImGui::PushID(name.c_str());	ImGui::Indent();
	ImGui::detail::AutoContainerPushFrontButton(var); ImGui::SameLine();
	ImGui::detail::AutoContainerPushBackButton(var);
	ImGui::detail::AutoContainerPopFrontButton(var); if (!var.empty()) ImGui::SameLine();
	ImGui::detail::AutoContainerPopBackButton(var);
	ImGui::PopID();					ImGui::Unindent();
}
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::list<T>)
	ImGui::detail::AutoContainerValues< const std::list<T> >("List " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _LIST_

#ifdef _DEQUE_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::deque<T>)
	if(ImGui::detail::AutoContainerValues< std::deque<T> >("Deque " + name, var))
	{
		ImGui::PushID(name.c_str());	ImGui::Indent();
		ImGui::detail::AutoContainerPushFrontButton(var); ImGui::SameLine();
		ImGui::detail::AutoContainerPushBackButton(var);
		ImGui::detail::AutoContainerPopFrontButton(var); if (!var.empty()) ImGui::SameLine();
		ImGui::detail::AutoContainerPopBackButton(var);
		ImGui::PopID();					ImGui::Unindent();
	}
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::deque<T>)
	ImGui::detail::AutoContainerValues< const std::deque<T> >("Deque " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _DEQUE_

#ifdef _FORWARD_LIST_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::forward_list<T>)
	if(ImGui::detail::AutoContainerValues< std::forward_list<T> >("Forward list " + name, var))
	{
		ImGui::PushID(name.c_str());	ImGui::Indent();
		ImGui::detail::AutoContainerPushFrontButton(var); if (!var.empty()) ImGui::SameLine();
		ImGui::detail::AutoContainerPopFrontButton(var); 
		ImGui::PopID();					ImGui::Unindent();
	}
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::forward_list<T>)
	ImGui::detail::AutoContainerValues< const std::forward_list<T> >("Forward list " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _FORWARD_LIST_

#ifdef _SET_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::set<T>)
	ImGui::detail::AutoContainerValues< std::set<T> >("Set " + name, var);
//todo insert
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::set<T>)
	ImGui::detail::AutoContainerValues< const std::set<T> >("Set " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _SET_

#ifdef _UNORDERED_SET_
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, std::unordered_set<T>)
	ImGui::detail::AutoContainerValues< std::unordered_set<T> >("Unordered set " + name, var);
//todo insert
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN(template<typename T>, const std::unordered_set<T>)
	ImGui::detail::AutoContainerValues< const std::unordered_set<T> >("Unordered set " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _UNORDERED_SET_

#ifdef _MAP_
IMGUI_AUTO_DEFINE_BEGIN_P((template<typename K, typename V>), (std::map<K, V>))
	ImGui::detail::AutoMapContainerValues< std::map<K,V> >("Map " + name, var);
//todo insert
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN_P((template<typename K, typename V>), (const std::map<K, V>))
	ImGui::detail::AutoMapContainerValues< const std::map<K,V> >("Map " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _MAP_

#ifdef _UNORDERED_MAP_
IMGUI_AUTO_DEFINE_BEGIN_P((template<typename K, typename V>), (std::unordered_map<K, V>))
	ImGui::detail::AutoMapContainerValues< std::unordered_map<K,V> >("Unordered map " + name, var);
//todo insert
IMGUI_AUTO_DEFINE_END
IMGUI_AUTO_DEFINE_BEGIN_P((template<typename K, typename V>), (const std::unordered_map<K, V>))
	ImGui::detail::AutoMapContainerValues< const std::unordered_map<K,V> >("Unordered map " + name, var);
IMGUI_AUTO_DEFINE_END
#endif // _UNORDERED_MAP_

#pragma endregion

#pragma region FUNCTIONS

IMGUI_AUTO_DEFINE_INLINE(template<>, std::add_pointer_t<void()>, if(ImGui::Button(name.c_str())) var(); )
IMGUI_AUTO_DEFINE_INLINE(template<>, const std::add_pointer_t<void()>, if(ImGui::Button(name.c_str())) var(); )

#pragma endregion
