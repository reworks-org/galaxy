///
/// Memory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_MEMORY_HPP_
#define GALAXY_META_MEMORY_HPP_

#include <type_traits>
#include <optional>
#include <string>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>

#include "galaxy/meta/Concepts.hpp"

#include <mimalloc-override.h>

namespace galaxy
{
	namespace meta
	{
		///
		/// \brief Reference wrapper.
		///
		/// Stores a pointer that can only be accessed as a reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<meta::is_object Type>
		using ref = std::reference_wrapper<Type>;

		///
		/// Optional reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<typename Type>
		using optional_ref = std::optional<std::reference_wrapper<Type>>;

		///
		/// std::string with faster allocation.
		///
		template<typename T>
		using vector = std::vector<T, mi_stl_allocator<T>>;

		///
		/// std::string with faster allocation.
		///
		template<typename T>
		using deque = std::deque<T, mi_stl_allocator<T>>;

		///
		/// std::string with faster allocation.
		///
		template<typename T>
		using forward_list = std::forward_list<T, mi_stl_allocator<T>>;

		///
		/// std::string with faster allocation.
		///
		template<typename T>
		using list = std::list<T, mi_stl_allocator<T>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename Compare = std::less<Key>>
		using set = std::set<Key, Compare, mi_stl_allocator<Key>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename T, typename Compare = std::less<Key>>
		using map = std::map<Key, T, Compare, mi_stl_allocator<std::pair<const Key, T>>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename Compare = std::less<Key>>
		using multiset = std::multiset<Key, Compare, mi_stl_allocator<Key>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename T, typename Compare = std::less<Key>>
		using multimap = std::multimap<Key, T, Compare, mi_stl_allocator<Key>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
		using unordered_set = std::unordered_set<Key, Hash, KeyEqual, mi_stl_allocator<Key>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
		using unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, mi_stl_allocator<std::pair<const Key, T>>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
		using unordered_multiset = std::unordered_multiset<Key, Hash, KeyEqual, mi_stl_allocator<Key>>;

		///
		/// std::string with faster allocation.
		///
		template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
		using unordered_multimap = std::unordered_multimap<Key, T, Hash, KeyEqual, mi_stl_allocator<std::pair<const Key, T>>>;

		///
		/// std::string with faster allocation.
		///
		template<typename T, typename Container = meta::deque<T>>
		using stack = std::stack<T, Container>;

		///
		/// std::string with faster allocation.
		///
		template<typename T, typename Container = meta::deque<T>>
		using queue = std::queue<T, Container>;

		///
		/// std::string with faster allocation.
		///
		template<class T, class Container = meta::vector<T>, class Compare = std::less<typename Container::value_type>>
		using priority_queue = std::priority_queue<T, Container, Compare>;
	} // namespace meta
} // namespace galaxy

#endif