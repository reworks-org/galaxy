///
/// ResourceCacheTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/res/ResourceCache.hpp>

struct DemoRes
{
	DemoRes(int val = 1)
	{
		m_val = val;
	}

	int m_val;
};

struct Cache : public pr::ResourceCache<DemoRes>
{
	Cache() noexcept
	    : ResourceCache()
	{
	}

	~Cache()
	{
		clear();
	}

	void clear() override
	{
		m_resources.clear();
	}
};

TEST(ResourceCache, create)
{
	Cache c;

	auto* res = c.create("demoA");
	EXPECT_EQ(res->m_val, 1);

	res = c.create("demoB", 2);

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 2);
}

TEST(ResourceCache, move)
{
	Cache c;
	DemoRes dr(5);

	auto* res = c.move("test", dr);

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 5);
}

TEST(ResourceCache, create_get)
{
	Cache c;

	auto* discard = c.create("test");
	auto* res     = c.get("test");

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 10);
}

TEST(ResourceCache, move_get)
{
	Cache c;
	DemoRes dr(7);

	c.move("test", dr);
	auto* res = c.get("test");

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 7);
}

TEST(ResourceCache, empty_get)
{
	Cache c;

	auto* res = c.get("test");
	EXPECT_EQ(res, nullptr);
}