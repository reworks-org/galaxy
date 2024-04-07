#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct Trie
{
	struct Node
	{
		std::unordered_map<char, Node*> children;
		bool isWordEnd = false;
	};
	static void Insert(Node* root, const std::string& key);
	static bool IsLastNode(struct Node* root);
	static void SuggestionsRec(Node* root, const std::string& currPrefix, std::vector<std::string>& suggestionsOut, int maxCount = -1);
	static int GetSuggestions(Node* root, const std::string& query, std::vector<std::string>& suggestionsOut, int maxCount = -1);

	static void Free(Node* root);
};