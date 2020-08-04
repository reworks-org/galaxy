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
	TextEditor::LanguageDefinition get_json_definition()
	{
		static bool init = false;
		static TextEditor::LanguageDefinition lang_def;

		if (!init)
		{
			for (const auto& k : {
				 "{", "}", "[", "]", ":", ",", "true", "false", "null", "'", "\"", "\\", "//"})
				lang_def.mKeywords.insert(k);

			lang_def.mCommentStart      = {"/*"};
			lang_def.mCommentEnd        = {"*/"};
			lang_def.mSingleLineComment = {"///"};

			lang_def.mCaseSensitive   = true;
			lang_def.mAutoIndentation = true;

			lang_def.mName = "JSON";

			init = true;
		}

		return lang_def;
	}
} // namespace galaxy