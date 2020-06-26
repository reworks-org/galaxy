///
/// ComponentSet.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <gtest/gtest.h>
#include <solar/sets/ComponentSet.hpp>

struct DemoStruct
{
	DemoStruct()
		:m_val("DemoStruct")
	{
	}

	DemoStruct(const std::string& val)
	{
		m_val = val;
	}

	void print()
	{
		std::cout << m_val << std::endl;
	}

	std::string m_val;
};

TEST(ComponentSet, AddADefaultComponent)
{
	sr::ComponentSet<DemoStruct> set;
	constexpr sr::Entity a = 1;

	auto* demo = set.add(a);

	EXPECT_FALSE(demo == nullptr);
	EXPECT_EQ(demo->m_val, "DemoStruct");
	EXPECT_EQ(set.getComponentArray().size(), 1);
}

TEST(ComponentSet, AddAnArgumentComponent)
{
	sr::ComponentSet<DemoStruct> set;
	constexpr sr::Entity b = 2;

	auto* demo = set.add(b, "ArgConstruct");

	EXPECT_FALSE(demo == nullptr);
	EXPECT_EQ(demo->m_val, "ArgConstruct");
	EXPECT_EQ(set.getComponentArray().size(), 1);
}

TEST(ComponentSet, GetAdded)
{
	sr::ComponentSet<DemoStruct> set;
	constexpr sr::Entity c = 3;

	set.add(c, "GetAdded");

	auto* demo = set.get(c);
	EXPECT_FALSE(demo == nullptr);
	EXPECT_EQ(demo->m_val, "GetAdded");
	EXPECT_EQ(set.getComponentArray().size(), 1);
}

TEST(ComponentSet, RemoveExisting)
{
	sr::ComponentSet<DemoStruct> set;
	constexpr sr::Entity d = 4;
	set.add(d);
	EXPECT_EQ(set.getComponentArray().size(), 1);

	set.remove(d);
	EXPECT_EQ(set.getComponentArray().size(), 0);
}

TEST(ComponentSet, RemoveNonExisting)
{
	sr::ComponentSet<DemoStruct> set;
	set.remove(1);
	EXPECT_EQ(set.getComponentArray().size(), 0);
}