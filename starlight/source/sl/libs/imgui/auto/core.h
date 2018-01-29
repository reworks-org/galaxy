#pragma once
#include <imgui/imgui.h>
#include <type_traits>
#include <string>

#if _MSC_VER == 1900
#	include <tuple>
#	include <typeinfo>
#	include "boost/pfr/flat/core.hpp"
#	define IMGUI_AUTO_STRUCT_TO_TUPLE boost::pfr::flat_structure_tie
#elif _MSC_VER > 1900
#	include <tuple>
#	include <typeinfo>
#	include "boost/pfr/precise/core.hpp"
#	define IMGUI_AUTO_STRUCT_TO_TUPLE boost::pfr::structure_tie
#endif

namespace ImGui
{
	//		IMGUI::AUTO() 
	//		=============

	// This is the function that this libary implements. (It's just a wrapper around the class ImGui::Auto_t<AnyType>)
	template<typename AnyType> void Auto(AnyType &anything, const std::string &name = std::string());

	//		Test function showcasing all the features of ImGui::Auto()
	void ShowAutoTestWindow();

	//		HELPER FUNCTIONS 
	//		================

	//same as std::as_const in c++17
	template <class T> constexpr	 std::add_const_t<T> & as_const(T& t) noexcept { return t;}

#pragma region DETAIL
namespace detail {

template<typename T>			bool AutoExpand(const std::string &name, T& value);
template<typename Container>	bool AutoContainerTreeNode(const std::string &name, Container &cont);
template<typename Container>	bool AutoContainerValues(const std::string &name, Container &cont); //Container must have .size(), .begin() and .end() methods and ::value_type.
template<typename Container>	bool AutoMapContainerValues(const std::string &name, Container &map); // Same as above but that iterates over pairs
template<typename Container>	void AutoContainerPushFrontButton(Container &cont);
template<typename Container>	void AutoContainerPushBackButton(Container &cont);
template<typename Container>	void AutoContainerPopFrontButton(Container &cont);
template<typename Container>	void AutoContainerPopBackButton(Container &cont);
template<typename Key, typename Value> void AutoMapKeyValue(Key & key, Value & value);

#ifdef _TUPLE_ //For tuples

template <class T> constexpr	 std::add_const_t<T>& as_const(T& t) noexcept { return t;} //same as std::as_const in c++17
template<std::size_t I, typename ...Args>		 void AutoTupleRecurse(std::tuple<Args...> &tpl, std::enable_if_t< 0 != I >* = 0);
template<std::size_t I, typename ...Args> inline void AutoTupleRecurse(std::tuple<Args...> &tpl, std::enable_if_t< 0 == I >* = 0){} //End of recursion.
template<std::size_t I, typename ...Args>		 void AutoTupleRecurse(const std::tuple<Args...> &tpl, std::enable_if_t< 0 != I >* = 0);
template<std::size_t I, typename ...Args> inline void AutoTupleRecurse(const std::tuple<Args...> &tpl, std::enable_if_t< 0 == I >* = 0){} //End of recursion.
template<typename ...Args> void AutoTuple(const std::string &name, std::tuple<Args...> &tpl);
template<typename ...Args> void AutoTuple(const std::string &name, const std::tuple<Args...> &tpl);
#endif // _TUPLE_

template<typename T, std::size_t N> using c_array_t = T[N];  //so arrays are regular types and can be used in macro

//template<typename Container> inline std::size_t AutoContainerSize(Container &cont, std::enable_if_t< std::is_array_v<Container>>*=0){	return sizeof(Container) / sizeof(decltype(*std::begin(cont)));}
//template<typename Container> inline std::size_t AutoContainerSize(Container &cont, std::enable_if_t<!std::is_array_v<Container>> *= 0) { return cont.size(); }

} // namespace detail
#pragma endregion

	//		PRIMARY TEMPLATE
	//		================
	// This implements the struct to tuple scenario
	template<typename AnyType> struct Auto_t { static void Auto(AnyType& anything, const std::string &name)
		{
#ifndef IMGUI_AUTO_STRUCT_TO_TUPLE
			static_assert(false, "TODO: fix for this compiler! (at least C++14 is required)")
#endif
			static_assert(!std::is_reference_v<AnyType> && std::is_copy_constructible_v<std::remove_all_extents_t<AnyType>> && !std::is_polymorphic_v<AnyType> &&
						  boost::pfr::detail::is_aggregate_initializable_n<AnyType,
								boost::pfr::detail::detect_fields_count_dispatch<AnyType>(boost::pfr::detail::size_t_<sizeof(AnyType)*8>{}, 1L)
						  >::value,		// If the above is not a constexpr expression, you are yousing an invalid type
						  "This type cannot be converted to a tuple.");
			auto & tuple = IMGUI_AUTO_STRUCT_TO_TUPLE(anything);
			ImGui::detail::AutoTuple("Struct " + name, tuple);
		}}; //ImGui::Auto_t<>::Auto()
} // namepspace ImGui

// Implementation of ImGui::Auto()
template<typename AnyType> inline void ImGui::Auto(AnyType &anything, const std::string &name)
{
	ImGui::Auto_t<AnyType>::Auto(anything, name);
}

	//		HELPER FUNCTIONS
	//		================

#pragma region UTILS

template<typename T> bool ImGui::detail::AutoExpand(const std::string &name, T& value)
{
	if (sizeof(T) <= IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE)
	{
		ImGui::PushID(name.c_str());
		ImGui::Bullet();
		ImGui::Auto_t<T>::Auto(value, name);
		ImGui::PopID();
		return true;
	}
	else if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Auto_t<T>::Auto(value, name);
		ImGui::TreePop();
		return true;
	}
	else return false;
}

template<typename Container> bool ImGui::detail::AutoContainerTreeNode(const std::string & name, Container & cont)
{
	//std::size_t size = ImGui::detail::AutoContainerSize(cont);
	std::size_t size = cont.size();
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		size_t elemsize = sizeof(decltype(*std::begin(cont)));
		ImGui::Text("size = %d, non dynamic elemsize = %d bytes", size, elemsize);
		return true;
	}
	else
	{
		float label_width = CalcTextSize(name.c_str()).x + ImGui::GetTreeNodeToLabelSpacing() + 5;
		std::string sizetext = "(size = " + std::to_string(size) + ')';
		float sizet_width = CalcTextSize(sizetext.c_str()).x;
		float avail_width = ImGui::GetContentRegionAvailWidth();
		if (avail_width > label_width + sizet_width)
		{
			ImGui::SameLine(avail_width - sizet_width);
			ImGui::TextUnformatted(sizetext.c_str());
		}
		return false;
	}
}
template<typename Container> bool ImGui::detail::AutoContainerValues(const std::string &name, Container &cont)
{
	if (ImGui::detail::AutoContainerTreeNode(name, cont))
	{
		ImGui::Indent();
		ImGui::PushID(name.c_str());
		std::size_t i = 0;
		for (auto & elem : cont)
		{
			std::string itemname = "[" + std::to_string(i)+ ']';
			ImGui::detail::AutoExpand(itemname, elem);
			++i;
		}
		ImGui::PopID();
		ImGui::Unindent();
		return true;
	}
	else return false;
}
template<typename Container> bool ImGui::detail::AutoMapContainerValues(const std::string &name, Container &cont)
{
	if (ImGui::detail::AutoContainerTreeNode(name, cont))
	{
		ImGui::Indent();
		std::size_t i = 0;
		for (auto & elem : cont)
		{
			ImGui::PushID(i);
			AutoMapKeyValue(elem.first, elem.second);
			ImGui::PopID();
			++i;
		}
		ImGui::Unindent();
		return true;
	}
	else return false;
}
template<typename Container> void ImGui::detail::AutoContainerPushFrontButton(Container &cont)	{ if(ImGui::SmallButton("Push Front")) cont.emplace_front(); }
template<typename Container> void ImGui::detail::AutoContainerPushBackButton(Container &cont)	{ if(ImGui::SmallButton("Push Back ")) cont.emplace_back(); }
template<typename Container> void ImGui::detail::AutoContainerPopFrontButton(Container &cont)	{ if(!cont.empty() && ImGui::SmallButton("Pop Front ")) cont.pop_front(); }
template<typename Container> void ImGui::detail::AutoContainerPopBackButton(Container &cont)	{ if(!cont.empty() && ImGui::SmallButton("Pop Back  ")) cont.pop_back(); }
template<typename Key, typename Value> void ImGui::detail::AutoMapKeyValue(Key &key, Value &value)
{
	bool b_k = sizeof(Key) <= IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE;
	bool b_v = sizeof(Value) <= IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE;
	if (b_k)
	{
		ImGui::TextUnformatted("[");	ImGui::SameLine();
		ImGui::Auto_t<Key>::Auto(key,"");		ImGui::SameLine();
		ImGui::TextUnformatted("]");	if (b_v) ImGui::SameLine();
		ImGui::Auto_t<Value>::Auto(value,"Value");
	}
	else
	{
		ImGui::Auto_t<Key>::Auto(key,"Key");
		ImGui::Auto_t<Value>::Auto(value, "Value");
	}
}

template<std::size_t I, typename ...Args> void ImGui::detail::AutoTupleRecurse(std::tuple<Args...> &tpl, std::enable_if_t< 0 != I >*)
{
	ImGui::detail::AutoTupleRecurse<I-1, Args...>(tpl); // first draw smaller indeces
	using type = decltype(std::get<I - 1>(tpl));
	std::string str ='<' + std::to_string(I) + ">: " + (std::is_const_v<type> ? "const ":"") +  typeid(type).name();
	ImGui::detail::AutoExpand(str, std::get<I-1>(tpl));
}
template<std::size_t I, typename ...Args> void ImGui::detail::AutoTupleRecurse(const std::tuple<Args...> &tpl, std::enable_if_t< 0 != I >*)
{
	ImGui::detail::AutoTupleRecurse<I-1, const Args...>(tpl); // first draw smaller indeces
	using type = decltype(std::get<I - 1>(tpl));
	std::string str ='<' + std::to_string(I) + ">: " + "const " +  typeid(type).name();
	ImGui::detail::AutoExpand(str, ImGui::as_const(std::get<I-1>(tpl)));
}
template<typename ...Args> void ImGui::detail::AutoTuple(const std::string &name, std::tuple<Args...> &tpl)
{
	constexpr std::size_t tuple_size = sizeof(decltype(tpl));
	constexpr std::size_t tuple_numelems = sizeof...(Args);
	if (tuple_size <= IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE && tuple_numelems <= IMGUI_AUTO_TREE_MAX_TUPLE_ELEMENTS)
	{
		ImGui::TextUnformatted((name + " (" + std::to_string(tuple_size) +" bytes)").c_str());
		ImGui::PushID(name.c_str()); ImGui::Indent();
		ImGui::detail::AutoTupleRecurse< tuple_numelems, Args...>(tpl);
		ImGui::Unindent();	ImGui::PopID();
	}
	else if(ImGui::TreeNode((name + " (" + std::to_string(tuple_size) +" bytes)").c_str()))
	{
		ImGui::detail::AutoTupleRecurse< tuple_numelems, Args...>(tpl);
		ImGui::TreePop();
	}
}
template<typename ...Args> void ImGui::detail::AutoTuple(const std::string &name, const std::tuple<Args...> &tpl) //same but const
{
	constexpr std::size_t tuple_size = sizeof(std::tuple<Args...>);
	constexpr std::size_t tuple_numelems = sizeof...(Args);
	if (tuple_size <= IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE && tuple_numelems <= IMGUI_AUTO_TREE_MAX_TUPLE_ELEMENTS)
	{
		ImGui::TextUnformatted((name + " !(" + std::to_string(tuple_size) +" bytes)").c_str());
		ImGui::PushID(name.c_str()); ImGui::Indent();
		ImGui::detail::AutoTupleRecurse< tuple_numelems, Args...>(tpl);
		ImGui::Unindent();	ImGui::PopID();
	}
	else if(ImGui::TreeNode((name + " (" + std::to_string(tuple_size) +" bytes)").c_str()))
	{
		ImGui::detail::AutoTupleRecurse< tuple_numelems, Args...>(tpl);
		ImGui::TreePop();
	}
}

#pragma endregion

	//		HELPER MACROS
	//		=============

#define UNPACK( ... ) __VA_ARGS__ //for unpacking parentheses. It is needed for macro arguments with commmas
//Enclose templatespec, AND typespec in parentheses in this version. Useful if there are commas in the argument.
#define IMGUI_AUTO_DEFINE_BEGIN_P(templatespec, typespec) namespace ImGui{ UNPACK templatespec struct Auto_t<UNPACK typespec > { static void Auto(UNPACK typespec &var, const std::string &name) {
//If macro arguments have no commmas inside use this version without parentheses
#define IMGUI_AUTO_DEFINE_BEGIN(templatespec, typespec) IMGUI_AUTO_DEFINE_BEGIN_P((templatespec), (typespec)) //when there are no commas in types, use this without parentheses
#define IMGUI_AUTO_DEFINE_END }};}
#define IMGUI_AUTO_DEFINE_INLINE_P(template_spec, type_spec, code) IMGUI_AUTO_DEFINE_BEGIN_P(template_spec, type_spec) code IMGUI_AUTO_DEFINE_END
#define IMGUI_AUTO_DEFINE_INLINE(template_spec, type_spec, code) IMGUI_AUTO_DEFINE_INLINE_P((template_spec), (type_spec), code)

