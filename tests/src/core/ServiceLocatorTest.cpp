///
/// ServiceLocatorTest.cpp
/// Tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/core/ServiceLocator.hpp>

using namespace galaxy;

int var = 0;

class Blank
{
public:
	Blank()
	{
		var = 1;
	}

	Blank(int val)
	{
		var = val;
	}

	~Blank()
	{
		var = 2;
	}

	int inter;
};

struct Blank2
{
	Blank2()  = default;
	~Blank2() = default;
};

TEST(Core, ServiceLocatorMake)
{
	var = 0;

	auto& test = core::ServiceLocator<Blank>::make();
	ASSERT_TRUE(var == 1);

	core::ServiceLocator<Blank>::del();
}

TEST(Core, ServiceLocatorMakeConstructor)
{
	var = 0;

	auto& test = core::ServiceLocator<Blank>::make(3);
	ASSERT_TRUE(var == 3);

	core::ServiceLocator<Blank>::del();
}

TEST(Core, ServiceLocatorGetRef)
{
	var = 0;

	core::ServiceLocator<Blank>::make();
	auto& ref = core::ServiceLocator<Blank>::ref();
	ref.inter = 1;

	ASSERT_TRUE(core::ServiceLocator<Blank>::ref().inter == 1);

	core::ServiceLocator<Blank>::del();
}

TEST(Core, ServiceLocatorDelete)
{
	var = 0;

	core::ServiceLocator<Blank>::make();
	core::ServiceLocator<Blank>::del();

	ASSERT_TRUE(var == 2);
}

TEST(Core, ServiceLocatorGetEmptyRef)
{
	EXPECT_THROW({ auto& ref = core::ServiceLocator<Blank2>::ref(); }, std::runtime_error);
}