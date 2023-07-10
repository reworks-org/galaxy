#include "Trie.h"

// based on: https://www.geeksforgeeks.org/auto-complete-feature-using-trie/

#include <iostream>


void Trie::Insert(Node* root, const std::string& key)
{
	Node* pCrawl = root;

	for (int level = 0; level < key.length(); level++)
	{
		char index = key[level];

		if (pCrawl->children.find(index) == pCrawl->children.end())
			pCrawl->children[index] = new Node();

		pCrawl = pCrawl->children[index];
	}

	// mark last node as leaf
	pCrawl->isWordEnd = true;
}

bool Trie::IsLastNode(Node* root)
{
	if (root->children.size() > 0)
		return 0;
	return 1;
}

void Trie::SuggestionsRec(Node* root, const std::string& currPrefix, std::vector<std::string>& suggestionsOut, int maxCount)
{
	if (maxCount == suggestionsOut.size())
		return;

	// found a string in Trie with the given prefix
	if (root->isWordEnd)
		suggestionsOut.push_back(currPrefix);

	for (const auto& item : root->children)
	{
		// child node character value
		char child = item.first;
		SuggestionsRec(root->children[item.first], currPrefix + child, suggestionsOut, maxCount);
	}
}

int Trie::GetSuggestions(Node* root, const std::string& query, std::vector<std::string>& suggestionsOut, int maxCount)
{
	Node* pCrawl = root;
	for (char c : query)
	{
		// no string in the Trie has this prefix
		if (pCrawl->children.find(c) == pCrawl->children.end())
			return 0;

		pCrawl = pCrawl->children[c];
	}
	// If prefix is present as a word, but
	// there is no subtree below the last
	// matching node.
	if (IsLastNode(pCrawl))
	{
		suggestionsOut.push_back(query);
		return -1;
	}
	SuggestionsRec(pCrawl, query, suggestionsOut, maxCount);
	return 1;
}

void Trie::Free(Node* root)
{
	for (auto& item : root->children)
		Free(item.second);
	delete root;
}
