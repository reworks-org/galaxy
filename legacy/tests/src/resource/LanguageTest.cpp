///
/// LanguageTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/resource/Language.hpp>

using namespace galaxy;

TEST(Language, Translate)
{
	resource::Language lang;

	lang.load_mem("en_test", "lang = {} lang['test.first'] = 'a' lang['test.second'] = 'b'");
	lang.set("en_test");

	EXPECT_EQ(lang.translate("test.first"), "a");
	EXPECT_EQ(lang.translate("test.second"), "b");
}