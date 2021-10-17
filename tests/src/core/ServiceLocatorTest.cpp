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

TEST(ServiceLocator, Make)
{
	var = 0;

	auto test = core::ServiceLocator<Blank>::make();
	ASSERT_TRUE(var == 1);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, MakeConstructor)
{
	var = 0;

	auto test = core::ServiceLocator<Blank>::make(3);
	ASSERT_TRUE(var == 3);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, GetPtr)
{
	var = 0;

	core::ServiceLocator<Blank>::make();
	auto ptr = core::ServiceLocator<Blank>::ptr();

	ASSERT_TRUE(ptr != nullptr);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, GetRef)
{
	var = 0;

	core::ServiceLocator<Blank>::make();
	auto& ref = core::ServiceLocator<Blank>::ref();
	ref.inter = 1;

	ASSERT_TRUE(core::ServiceLocator<Blank>::ref().inter == 1);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, Delete)
{
	var = 0;

	core::ServiceLocator<Blank>::make();
	core::ServiceLocator<Blank>::del();

	ASSERT_TRUE(var == 2);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, GetEmptyPtr)
{
	var = 0;

	auto ptr = core::ServiceLocator<Blank>::ptr();
	ASSERT_TRUE(ptr == nullptr);

	core::ServiceLocator<Blank>::del();
}

TEST(ServiceLocator, GetEmptyRef)
{
	var = 0;

	EXPECT_THROW(
		{
			try
			{
				auto& ref = core::ServiceLocator<Blank>::ref();
			}
			catch (const std::runtime_error& e)
			{
				ASSERT_TRUE(std::strcmp(e.what(), ""));
				throw;
			}
		},
		std::runtime_error);

	core::ServiceLocator<Blank>::del();
}