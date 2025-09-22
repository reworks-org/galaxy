///
/// CacheTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/resource/Cache.hpp>

struct TestResource
{
	TestResource()
	{
		a = 1;
	}

	TestResource(int _a)
	{
		a = _a;
	}

	int a = 0;
};

using TestLoader = galaxy::Loader<TestResource>;

TEST(Cache, AddGet)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	cache.load_resource("test", 2);

	auto res = cache.get("test");
	ASSERT_EQ(res->a, 2);
}

TEST(Cache, Insert)
{
	auto ptr = std::make_shared<TestResource>(3);

	galaxy::Cache<TestResource, TestLoader> cache;
	cache.insert("test", ptr);

	auto res = cache.get("test");
	ASSERT_EQ(res->a, 3);
}

TEST(Cache, DefaultConstruct)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	cache.load_resource("test");

	auto res = cache.get("test");
	ASSERT_EQ(res->a, 1);
}

TEST(Cache, Has)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	cache.load_resource("test");

	ASSERT_TRUE(cache.has("test"));
}

TEST(Cache, Empty)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	ASSERT_TRUE(cache.empty());

	cache.load_resource("test");
	ASSERT_FALSE(cache.empty());
}

TEST(Cache, Clear)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	cache.load_resource("test");
	ASSERT_FALSE(cache.empty());

	cache.clear();
	ASSERT_TRUE(cache.empty());
}

TEST(Cache, Size)
{
	galaxy::Cache<TestResource, TestLoader> cache;
	cache.load_resource("test");

	ASSERT_EQ(cache.size(), 1);

	cache.clear();
	ASSERT_EQ(cache.size(), 0);
}
