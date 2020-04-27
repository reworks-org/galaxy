///
/// JSONDefinition.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "JSONDefinition.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	const TextEditor::LanguageDefinition& getJsonDefinition() noexcept
	{
		static bool inited = false;
		static TextEditor::LanguageDefinition langDef;

		if (!inited)
		{
			static const char* const keywords[] = {
				"{", "}", "[", "]", ":", ",", "true", "false", "null", "'", "\"", "\\", "//"
			};

			for (auto& k : keywords)
				langDef.mKeywords.insert(k);

			langDef.mCommentStart = "";
			langDef.mCommentEnd = "";
			langDef.mSingleLineComment = "";

			langDef.mCaseSensitive = true;
			langDef.mAutoIndentation = true;

			langDef.mName = "JSON";

			inited = true;
		}

		return langDef;
	}
}