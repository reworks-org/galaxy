///
/// CacheTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/resource/Cache.hpp>

struct TestRes
{
	TestRes(int _val)
	{
		value = _val;
	}

	int value;
};

struct Loader final : public galaxy::resource::Loader<TestRes>
{
	std::shared_ptr<TestRes> create(std::string_view file) override
	{
		auto ptr = std::make_shared<TestRes>(1);
		return ptr;
	}

	galaxy::resource::Holder<TestRes> create_from_json(std::string_view json_file) override
	{
		galaxy::resource::Holder<TestRes> h;
		h["test"] = std::make_shared<TestRes>(2);

		return h;
	}
};

TEST(Resource, EmptyCache)
{
	using TestCache = galaxy::resource::Cache<TestRes, Loader>;
	TestCache cache;

	EXPECT_TRUE(cache.empty());
	EXPECT_FALSE(cache.has("invalid"));
	EXPECT_EQ(cache.get("invalid"), nullptr);
}

TEST(Resource, CacheCreate)
{
	using TestCache = galaxy::resource::Cache<TestRes, Loader>;
	TestCache cache;

	auto res = cache.create("test", "notneeded");
	ASSERT_TRUE(res != nullptr);

	EXPECT_TRUE(cache.has("test"));
	EXPECT_FALSE(cache.empty());
}

TEST(Resource, CacheBatchCreate)
{
	using TestCache = galaxy::resource::Cache<TestRes, Loader>;
	TestCache cache;

	cache.create_from_json("notneeded");

	auto res = cache.get("test");
	ASSERT_TRUE(res != nullptr);

	EXPECT_TRUE(cache.has("test"));
	EXPECT_FALSE(cache.empty());
}

TEST(Resource, CacheClear)
{
	using TestCache = galaxy::resource::Cache<TestRes, Loader>;
	TestCache cache;

	cache.create_from_json("notneeded");

	cache.clear();

	auto res = cache.get("test");
	ASSERT_TRUE(res == nullptr);

	EXPECT_FALSE(cache.has("test"));
	EXPECT_TRUE(cache.empty());
}