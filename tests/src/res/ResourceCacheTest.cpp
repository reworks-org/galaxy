///
/// ResourceCacheTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <galaxy/res/ResourceCache.hpp>

struct DemoRes
{
	DemoRes(int val = 1)
	{
		m_val = val;
	}

	int m_val;
};

struct Cache : public galaxy::res::ResourceCache<DemoRes>
{
	Cache()
	    : ResourceCache()
	{
	}

	virtual ~Cache()
	{
		clear();
	}

	void clear() noexcept override
	{
		m_resources.clear();
	}
};

TEST(ResourceCache, Create)
{
	Cache c;

	auto* res = c.create("demoA");
	EXPECT_EQ(res->m_val, 1);

	res = c.create("demoB", 2);

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 2);
}

TEST(ResourceCache, CreateGet)
{
	Cache c;

	auto* discard = c.create("test", 10);
	auto* res     = c.get("test");

	ASSERT_TRUE(res != nullptr);
	EXPECT_EQ(res->m_val, 10);
}

TEST(ResourceCache, EmptyGet)
{
	Cache c;

	auto* res = c.get("test");
	EXPECT_EQ(res, nullptr);
}