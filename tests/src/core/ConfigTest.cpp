///
/// ConfigTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/core/Config.hpp>

TEST(Config, SetGet)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar");

	ASSERT_EQ(c.get<std::string>("foo"), "bar");
}

TEST(Config, SetGetSections)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello");

	ASSERT_EQ(c.get<std::string>("foo", "hello"), "bar");
}

TEST(Config, SetGetMultiSection)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello.world");

	ASSERT_EQ(c.get<std::string>("foo", "hello.world"), "bar");
}

TEST(Config, SetGetMultiSectionDelim)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello:world", ":");

	ASSERT_EQ(c.get<std::string>("foo", "hello:world", ":"), "bar");
}

TEST(Config, Restore)
{
	galaxy::Config c;
	c.restore<std::string>("foo", "bar");

	ASSERT_EQ(c.get<std::string>("foo"), "bar");
}

TEST(Config, RestoreAlreadyExisting)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar");
	c.restore<std::string>("foo", "hello world");

	ASSERT_EQ(c.get<std::string>("foo"), "bar");
}

TEST(Config, RestoreSections)
{
	galaxy::Config c;
	c.restore<std::string>("foo", "bar", "hello");

	ASSERT_EQ(c.get<std::string>("foo", "hello"), "bar");
}

TEST(Config, RestoreMultiSection)
{
	galaxy::Config c;
	c.restore<std::string>("foo", "bar", "hello.world");

	ASSERT_EQ(c.get<std::string>("foo", "hello.world"), "bar");
}

TEST(Config, RestoreSectionsAlreadyExisting)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello");
	c.restore<std::string>("foo", "wrong", "hello");

	ASSERT_EQ(c.get<std::string>("foo", "hello"), "bar");
}

TEST(Config, RestoreMultiSectionAlreadyExisting)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello.world");
	c.restore<std::string>("foo", "wrong", "hello.world");

	ASSERT_EQ(c.get<std::string>("foo", "hello.world"), "bar");
}

TEST(Config, RestoreMultiSectionDelim)
{
	galaxy::Config c;
	c.restore<std::string>("foo", "bar", "hello:world", ":");

	ASSERT_EQ(c.get<std::string>("foo", "hello:world", ":"), "bar");
}

TEST(Config, Has)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar");

	ASSERT_TRUE(c.has("foo"));
}

TEST(Config, HasNot)
{
	galaxy::Config c;
	ASSERT_FALSE(c.has("foo"));
}

TEST(Config, HasSections)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello");

	ASSERT_TRUE(c.has("foo", "hello"));
}

TEST(Config, HasNotSections)
{
	galaxy::Config c;
	ASSERT_FALSE(c.has("foo", "hello"));
}

TEST(Config, HasMultiSection)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello.world");

	ASSERT_TRUE(c.has("foo", "hello.world"));
}

TEST(Config, HasNotMultiSection)
{
	galaxy::Config c;
	ASSERT_FALSE(c.has("foo", "hello.world"));
}

TEST(Config, HasMultiSectionDelim)
{
	galaxy::Config c;
	c.set<std::string>("foo", "bar", "hello:world", ":");

	ASSERT_TRUE(c.has("foo", "hello:world", ":"));
}

TEST(Config, HasNotMultiSectionDelim)
{
	galaxy::Config c;
	ASSERT_FALSE(c.has("foo", "hello:world", ":"));
}

TEST(Config, Empty)
{
	galaxy::Config c;
	ASSERT_TRUE(c.empty());
}
